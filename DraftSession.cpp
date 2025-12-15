#include "DraftSession.h"
#include <filesystem>
#include <optional>
#include <algorithm>
#include <random>
#include <iostream>
#include "Exception.h"
using namespace std;

DraftSession::DraftSession(sf::RenderWindow& win, const Formation& f)
    : formation(f),
      team(f),
      db(),
      positionMap{
          {"GK","GK"},{"LB","LB"},{"LCB","CB"},{"RCB","CB"},{"RB","RB"},
          {"LCM","CM"},{"CDM","CM"},{"RCM","CM"},{"LM","LM"},{"RM","RM"},
          {"LW","LW"},{"RW","RW"},{"ST","ST"},{"LST","ST"},{"RST","ST"},
          {"CB","CB"}
      },

      window(win),
      font(),
      backgroundTexture(),
      backgroundSprite(backgroundTexture),
      defaultCardTexture(),
      defaultManagerTexture(),
      sidebar(),
      dummyTexture(),
      ratingDisplay(font),
      chemistryDisplay(font),
      statsBackground(),
      currentPositionIndex(0),
      isDrafting(true),
      draftCompleted(false),
      choosingManager(false),
      currentOptions(),
      sidebarVisuals(),
      previewSprite(dummyTexture),
      selectedSwapIndex(-1)
{
    sidebarVisuals.reserve(20);
    sf::Texture dummy;
    previewSprite.setTexture(dummy);

    loadResources();
    generateOptions();
}

void DraftSession::loadResources() {
    if (!font.openFromFile("arial.ttf")) {
        throw FileMissingException("arial.ttf");
    }


    ratingDisplay.setFont(font);
    chemistryDisplay.setFont(font);

    db.loadAll();

    if (!defaultCardTexture.loadFromFile("images/players/card_default.png")) {
        cerr << "Eroare: Nu am gasit images/players/card_default.png! Voi folosi un patrat alb.\n";
        if (!defaultCardTexture.resize(sf::Vector2u(100, 100))) {
            cerr << "Critical: Nu am putut crea textura default (resize failed).\n";
        }
    }
    defaultCardTexture.setSmooth(true);

    string path = "images/formations/" + formation.getName() + ".png";
    if (backgroundTexture.loadFromFile(path)) {
        backgroundTexture.setSmooth(true);
        backgroundSprite.setTexture(backgroundTexture, true);

        sf::FloatRect bgBounds = backgroundSprite.getLocalBounds();
        backgroundSprite.setOrigin({bgBounds.size.x/2.0f, bgBounds.size.y/2.0f});
        backgroundSprite.setPosition({640.0f + 100.0f, 360.0f});

        float scaleY = 720.0f / bgBounds.size.y;
        backgroundSprite.setScale({scaleY, scaleY});
    }

    sidebar.setSize(sf::Vector2f(250.f, 720.f));
    sidebar.setFillColor(sf::Color(30, 30, 30, 240));

    if (!defaultManagerTexture.loadFromFile("images/managers/manager_default.png")) {
        if (!defaultManagerTexture.resize(sf::Vector2u(100, 100))) {
            cerr << "Critical: Nu am putut crea textura manager (resize failed).\n";
        }
    }
    defaultManagerTexture.setSmooth(true);

    statsBackground.setSize(sf::Vector2f(250.f, 80.f));
    statsBackground.setFillColor(sf::Color(0, 0, 0, 150));
    statsBackground.setOutlineColor(sf::Color::White);
    statsBackground.setOutlineThickness(2);
    statsBackground.setPosition({1010.f, 20.f});

    ratingDisplay.setCharacterSize(20);
    ratingDisplay.setFillColor(sf::Color::Yellow);
    ratingDisplay.setPosition({1030.f, 30.f});

    chemistryDisplay.setCharacterSize(20);
    chemistryDisplay.setFillColor(sf::Color::Cyan);
    chemistryDisplay.setPosition({1030.f, 60.f});

    updateStatsUI();
}

void DraftSession::generateManagerOptions() {
    currentOptions.clear();

    vector<Manager> candidates = db.getManagers();

    if (candidates.empty()) {
        cerr << "ATENTIE: Nu s-au gasit manageri in baza de date (MANAGER.txt lipsa sau gol)!\n";
    }

    static std::random_device rd;
    static std::mt19937 g(rd());
    std::shuffle(candidates.begin(), candidates.end(), g);


    int count = min(static_cast<int>(candidates.size()), 5);
    currentOptions.reserve(count);

    float cardW = 140.0f; float cardH = 200.0f; float gap = 20.0f;

    float totalW = (static_cast<float>(count) * cardW) + (static_cast<float>(count - 1) * gap);
    float startX = 250.0f + (1030.0f - totalW) / 2.0f;
    float startY = 260.0f;

    for (int i = 0; i < count; ++i) {
        currentOptions.emplace_back(font, dummyTexture);
        CardOption& card = currentOptions.back();

        card.manager = candidates[i];
        card.isManager = true;

        card.shape.setSize({cardW, cardH});
        card.shape.setFillColor(sf::Color(30, 40, 70, 220));
        card.shape.setOutlineColor(sf::Color(255, 215, 0));
        card.shape.setOutlineThickness(2);


        float posX = startX + static_cast<float>(i) * (cardW + gap);
        card.shape.setPosition({posX, startY});

        bool loadSuccess = card.texture.loadFromFile(card.manager.getImagePath());

        if (loadSuccess) {
            card.texture.setSmooth(true);
            card.sprite.setTexture(card.texture, true);
        } else {
            card.sprite.setTexture(defaultManagerTexture, true);
        }


        sf::FloatRect bounds = card.sprite.getLocalBounds();
        if (bounds.size.x > 0) {
            float scaleX = (cardW - 10.0f) / bounds.size.x;
            float scaleY = (cardH - 50.0f) / bounds.size.y;
            float scale = min(scaleX, scaleY);
            card.sprite.setScale({scale, scale});
            float spriteW = bounds.size.x * scale;
            card.sprite.setPosition({card.shape.getPosition().x + (cardW - spriteW) / 2.0f, startY + 10.0f});
        }


        card.nameText.setString(card.manager.getName());
        card.nameText.setCharacterSize(14);
        sf::FloatRect tr = card.nameText.getLocalBounds();
        card.nameText.setOrigin({tr.size.x/2.0f, 0.0f});
        card.nameText.setPosition({card.shape.getPosition().x + cardW/2.0f, startY + cardH - 25.0f});

        card.ratingText.setString("MNG");
        card.ratingText.setFillColor(sf::Color::Cyan);
        card.ratingText.setPosition({card.shape.getPosition().x + 5.0f, startY + 5.0f});
    }
}

void DraftSession::generateOptions() {
    currentOptions.clear();

    const vector<string>& positions = formation.getPositions();

    if (static_cast<size_t>(currentPositionIndex) >= positions.size()) {
        if (!choosingManager) {
            choosingManager = true;
            generateManagerOptions();
            return;
        } else {
            draftCompleted = true;
            return;
        }
    }

    string currentPos = positions[currentPositionIndex];
    string dbGroup = positionMap[currentPos];

    const auto& allCandidates = db.getPlayersByPosition(dbGroup);


    vector<Player*> validCandidates;
    validCandidates.reserve(allCandidates.size());

    for (const auto& uPtr : allCandidates) {

        if (!team.isPlayerInTeam(*uPtr)) {
            validCandidates.push_back(uPtr.get());
        }
    }


    static std::random_device rd;
    static std::mt19937 g(rd());
    std::shuffle(validCandidates.begin(), validCandidates.end(), g);

    int count = min(static_cast<int>(validCandidates.size()), 5);


    float cardW = 140.0f; float cardH = 200.0f; float gap = 20.0f;
    float totalW = (static_cast<float>(count) * cardW) + (static_cast<float>(count - 1) * gap);
    float startX = 250.0f + (1030.0f - totalW) / 2.0f;
    float startY = 260.0f;

    currentOptions.reserve(count);

    for (int i = 0; i < count; ++i) {
        currentOptions.emplace_back(font, dummyTexture);
        CardOption& card = currentOptions.back();

        card.playerPtr = validCandidates[i]->clone();
        Player& pRef = *card.playerPtr;

        card.shape.setSize({cardW, cardH});
        card.shape.setFillColor(sf::Color(40, 40, 40, 200));
        card.shape.setOutlineColor(sf::Color::White);
        card.shape.setOutlineThickness(2);

        float posX = startX + static_cast<float>(i) * (cardW + gap);
        card.shape.setPosition({posX, startY});

        bool loadSuccess = false;
        if (!pRef.getImagePath().empty()) {
            loadSuccess = card.texture.loadFromFile(pRef.getImagePath());
        }

        if (loadSuccess) {
            card.texture.setSmooth(true);
            card.sprite.setTexture(card.texture, true);
        } else {
            card.sprite.setTexture(defaultCardTexture, true);
        }

        sf::FloatRect bounds = card.sprite.getLocalBounds();
        if (bounds.size.x > 0 && bounds.size.y > 0) {
            float scaleX = (cardW - 10.0f) / bounds.size.x;
            float scaleY = (cardH - 50.0f) / bounds.size.y;
            float scale = min(scaleX, scaleY);
            card.sprite.setScale({scale, scale});

            float spriteW = bounds.size.x * scale;
            card.sprite.setPosition({posX + (cardW - spriteW) / 2.0f, startY + 10.0f});
        }

        card.nameText.setString(pRef.getName());
        card.nameText.setCharacterSize(14);
        card.nameText.setFillColor(sf::Color::White);
        sf::FloatRect textRect = card.nameText.getLocalBounds();
        card.nameText.setOrigin({textRect.size.x/2.0f, 0.0f});
        card.nameText.setPosition({posX + cardW/2.0f, startY + cardH - 25.0f});

        card.ratingText.setString(to_string(pRef.getRating()));
        card.ratingText.setCharacterSize(18);
        card.ratingText.setFillColor(sf::Color::Yellow);
        card.ratingText.setOutlineColor(sf::Color::Black);
        card.ratingText.setOutlineThickness(1);
        card.ratingText.setPosition({posX + 5.0f, startY + 5.0f});
    }
}

void DraftSession::selectPlayer(int index) {

    if (index < 0 || static_cast<size_t>(index) >= currentOptions.size()) return;

    CardOption& choice = currentOptions[index];


    if (choice.isManager) {
        team.setManager(choice.manager);
        cout << "Manager Ales: " << choice.manager.getName() << "\n";

        const sf::Texture& texToCopy = (choice.texture.getSize().x > 0) ? choice.texture : defaultManagerTexture;
        sidebarVisuals.emplace_back(font, texToCopy);
        SelectedVisual& sv = sidebarVisuals.back();
        sv.sprite.setTexture(sv.texture, true);

        float sidebarY = 60.0f + 11.0f * 50.0f + 10.0f;

        sv.sprite.setScale({0.15f, 0.15f});
        sv.sprite.setPosition({20.0f, sidebarY});

        sv.info.setString("Manager\n" + choice.manager.getNationality());
        sv.info.setFillColor(sf::Color::Cyan);
        sv.info.setPosition({80.0f, sidebarY + 5.0f});

        updateStatsUI();
        draftCompleted = true;
        choosingManager = false;
        currentOptions.clear();
        return;
    }


    string posLabel = formation.getPositions()[currentPositionIndex];


    if (choice.playerPtr) {
        cout << "Ales: " << choice.playerPtr->getName() << "\n";

        int rating = choice.playerPtr->getRating();

        team.addPlayer(posLabel, std::move(choice.playerPtr));

        const sf::Texture& textureToCopy = (choice.texture.getSize().x > 0) ? choice.texture : defaultCardTexture;
        sidebarVisuals.emplace_back(font, textureToCopy);

        SelectedVisual& sv = sidebarVisuals.back();
        sv.sprite.setTexture(sv.texture, true);

        float sidebarY = 60.0f + static_cast<float>(currentPositionIndex) * 50.0f;

        sv.sprite.setScale({0.15f, 0.15f});
        sv.sprite.setPosition({20.0f, sidebarY});

        sv.info.setString(posLabel + "\n" + to_string(rating));
        sv.info.setCharacterSize(14);
        sv.info.setFillColor(sf::Color::White);
        sv.info.setPosition({80.0f, sidebarY + 5.0f});
    }

    updateStatsUI();
    currentPositionIndex++;
    generateOptions();
}

void DraftSession::updateStatsUI() {
    int rating = static_cast<int>(team.computeRating());
    int chem = team.computeChemistry();

    ratingDisplay.setString("RATING: " + to_string(rating));
    chemistryDisplay.setString("CHEMISTRY: " + to_string(chem));

    const auto& positions = formation.getPositions();

    for (size_t i = 0; i < sidebarVisuals.size(); ++i) {
        if (i < positions.size()) {
            string posLabel = positions[i];

            Player* pPtr = team.getPlayerOnPosition(posLabel);

            if (pPtr) {
                int indivChem = team.getPlayerChemistry(posLabel);


                string infoText = posLabel + "     Ch: " + to_string(indivChem) + "\n" +
                                  to_string(pPtr->getRating());

                sidebarVisuals[i].info.setString(infoText);

                if (indivChem == 10) sidebarVisuals[i].info.setFillColor(sf::Color::Green);
                else if (indivChem >= 7) sidebarVisuals[i].info.setFillColor(sf::Color::Yellow);
                else if (indivChem >= 4) sidebarVisuals[i].info.setFillColor(sf::Color(255, 165, 0)); // Orange
                else sidebarVisuals[i].info.setFillColor(sf::Color::Red);
            }
        }
    }
}

void DraftSession::handleInput() {
    while (const std::optional event = window.pollEvent()) {
        if (event->is<sf::Event::Closed>()) {
            window.close();
            isDrafting = false;
        }
        else if (const auto* kp = event->getIf<sf::Event::KeyPressed>()) {
            if (kp->code == sf::Keyboard::Key::Escape) {
                if (selectedSwapIndex != -1) {
                    sidebarVisuals[selectedSwapIndex].sprite.setScale({0.15f, 0.15f});
                    sidebarVisuals[selectedSwapIndex].sprite.setColor(sf::Color::White);
                    selectedSwapIndex = -1;
                } else {
                    isDrafting = false;
                }
            }
        }
        else if (const auto* mp = event->getIf<sf::Event::MouseButtonPressed>()) {
            if (mp->button == sf::Mouse::Button::Left) {
                sf::Vector2f mPos = window.mapPixelToCoords(sf::Mouse::getPosition(window));

                if (!draftCompleted) {
                    for (size_t i = 0; i < currentOptions.size(); ++i) {
                        if (currentOptions[i].shape.getGlobalBounds().contains(mPos)) {
                            selectPlayer(static_cast<int>(i));
                            break;
                        }
                    }
                }

                for (size_t i = 0; i < sidebarVisuals.size(); ++i) {
                    if (sidebarVisuals[i].sprite.getGlobalBounds().contains(mPos)) {
                        if (selectedSwapIndex == -1) {
                            selectedSwapIndex = static_cast<int>(i);
                            sidebarVisuals[i].sprite.setScale({0.18f, 0.18f});
                            sidebarVisuals[i].sprite.setColor(sf::Color(200, 255, 200));
                            cout << "Selectat pentru swap: Index " << i << "\n";
                        }
                        else if (selectedSwapIndex == static_cast<int>(i)) {
                            sidebarVisuals[i].sprite.setScale({0.15f, 0.15f});
                            sidebarVisuals[i].sprite.setColor(sf::Color::White);
                            selectedSwapIndex = -1;
                            cout << "Deselectat.\n";
                        }
                        else {
                            try {
                                size_t maxPlayers = formation.getPositions().size();


                                if (static_cast<size_t>(selectedSwapIndex) >= maxPlayers || i >= maxPlayers) {
                                    throw InvalidOperationException("Nu poti schimba Managerul cu un jucator!");
                                }

                                string pos1 = formation.getPositions()[selectedSwapIndex];
                                string pos2 = formation.getPositions()[i];

                                cout << "Incercare Swap: " << pos1 << " <-> " << pos2 << "\n";


                                team.swapPlayers(pos1, pos2);


                                std::swap(sidebarVisuals[selectedSwapIndex], sidebarVisuals[i]);


                                sidebarVisuals[selectedSwapIndex].sprite.setTexture(sidebarVisuals[selectedSwapIndex].texture);
                                sidebarVisuals[i].sprite.setTexture(sidebarVisuals[i].texture);


                                float sidebarY_Select = 60.0f + static_cast<float>(selectedSwapIndex) * 50.0f;
                                float sidebarY_Target = 60.0f + static_cast<float>(i) * 50.0f;

                                sidebarVisuals[selectedSwapIndex].sprite.setPosition({20.0f, sidebarY_Select});
                                sidebarVisuals[selectedSwapIndex].info.setPosition({80.0f, sidebarY_Select + 5.0f});

                                sidebarVisuals[i].sprite.setPosition({20.0f, sidebarY_Target});
                                sidebarVisuals[i].info.setPosition({80.0f, sidebarY_Target + 5.0f});

                                updateStatsUI();
                                cout << "Swap reusit!\n";

                            } catch (const InvalidOperationException& e) {
                                cerr << "SWAP ERROR: " << e.what() << "\n";
                            }

                            for(auto& sv : sidebarVisuals) {
                                sv.sprite.setScale({0.15f, 0.15f});
                                sv.sprite.setColor(sf::Color::White);
                            }
                            selectedSwapIndex = -1;
                        }
                    }
                }
            }
        }
    }
}

void DraftSession::run() {
    while (isDrafting && window.isOpen()) {
        handleInput();
        draw();
    }
}

void DraftSession::draw() {
    window.clear(sf::Color(20, 20, 20));

    window.draw(backgroundSprite);
    window.draw(sidebar);

    sf::Text title(font, "Echipa Ta", 24);
    title.setPosition({20.0f, 20.0f});
    window.draw(title);

    for (const auto& item : sidebarVisuals) {
        window.draw(item.sprite);
        window.draw(item.info);
    }

    window.draw(statsBackground);
    window.draw(ratingDisplay);
    window.draw(chemistryDisplay);

    if (!draftCompleted) {
        string titleText;
        if (choosingManager) {
            titleText = "Alege Managerul";
        } else {

            if (static_cast<size_t>(currentPositionIndex) < formation.getPositions().size()) {
                titleText = "Alege: " + formation.getPositions()[currentPositionIndex];
            } else {
                titleText = "Finalizare...";
            }
        }

        sf::Text pickText(font, titleText, 30);
        pickText.setFillColor(sf::Color::White);
        pickText.setOutlineColor(sf::Color::Black);
        pickText.setOutlineThickness(2);
        sf::FloatRect tr = pickText.getLocalBounds();
        pickText.setOrigin({tr.size.x/2.0f, 0.0f});
        pickText.setPosition({765.0f, 200.0f});
        window.draw(pickText);

        sf::Vector2f mPos = window.mapPixelToCoords(sf::Mouse::getPosition(window));
        bool showPreview = false;

        for (auto& opt : currentOptions) {
            if (opt.shape.getGlobalBounds().contains(mPos)) {
                opt.shape.setOutlineColor(sf::Color::Yellow);

                if (sf::Mouse::isButtonPressed(sf::Mouse::Button::Right)) {
                    showPreview = true;

                    const sf::Texture* fallbackTexture = &defaultCardTexture;
                    if (opt.isManager) {
                        fallbackTexture = &defaultManagerTexture;
                    }

                    const sf::Texture& textureToShow = (opt.texture.getSize().x > 0) ? opt.texture : *fallbackTexture;

                    previewSprite.setTexture(textureToShow, true);

                    sf::FloatRect bounds = previewSprite.getLocalBounds();
                    previewSprite.setOrigin({bounds.size.x/2.0f, bounds.size.y/2.0f});
                    previewSprite.setPosition({640.0f, 360.0f});

                    if (textureToShow.getSize().x < 200) {
                        previewSprite.setScale({3.0f, 3.0f});
                    } else {
                        previewSprite.setScale({1.0f, 1.0f});
                    }
                }
            } else {
                opt.shape.setOutlineColor(sf::Color::White);
            }

            window.draw(opt.shape);
            window.draw(opt.sprite);
            window.draw(opt.nameText);
            window.draw(opt.ratingText);
        }

        if (showPreview) {
            sf::RectangleShape overlay({1280.0f, 720.0f});
            overlay.setFillColor(sf::Color(0,0,0,200));
            window.draw(overlay);
            window.draw(previewSprite);
        }
    } else {
        sf::Text doneText(font, "ECHIPA COMPLETA!", 50);
        doneText.setFillColor(sf::Color::Green);
        sf::FloatRect dBounds = doneText.getLocalBounds();
        doneText.setOrigin({dBounds.size.x/2.0f, dBounds.size.y/2.0f});
        doneText.setPosition({765.0f, 300.0f});
        window.draw(doneText);
    }

    window.display();
}