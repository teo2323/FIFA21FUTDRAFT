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
      overallDisplay(font),
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
    sidebarVisuals.reserve(12);
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
        backgroundSprite.setPosition({765.0f, 360.0f});

        float scaleY = 720.0f / bgBounds.size.y;
        backgroundSprite.setScale({scaleY, scaleY});
    }

    sidebar.setSize(sf::Vector2f(250.f, 720.f));
    sidebar.setFillColor(sf::Color(30, 30, 30, 255));

    if (!defaultManagerTexture.loadFromFile("images/managers/manager_default.png")) {
        if (!defaultManagerTexture.resize(sf::Vector2u(100, 100))) {
            cerr << "Critical: Nu am putut crea textura manager (resize failed).\n";
        }
    }
    defaultManagerTexture.setSmooth(true);

    statsBackground.setSize(sf::Vector2f(250.f, 100.f));
    statsBackground.setFillColor(sf::Color(0, 0, 0, 180));
    statsBackground.setOutlineColor(sf::Color::White);
    statsBackground.setOutlineThickness(1);
    statsBackground.setPosition({280.f, 20.f});

    ratingDisplay.setCharacterSize(18);
    ratingDisplay.setFillColor(sf::Color::Yellow);
    ratingDisplay.setPosition({280.f, 25.f});

    chemistryDisplay.setCharacterSize(18);
    chemistryDisplay.setFillColor(sf::Color::Cyan);
    chemistryDisplay.setPosition({280.f, 50.f});

    overallDisplay.setCharacterSize(18);
    overallDisplay.setFillColor(sf::Color::Green);
    overallDisplay.setPosition({280.f, 75.f});

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

    float startX = 765.0f - (totalW / 2.0f);
    float startY = 260.0f;

    for (int i = 0; i < count; ++i) {
        currentOptions.emplace_back(font, dummyTexture);
        CardOption& card = currentOptions.back();

        card.manager = candidates[i];
        card.isManager = true;

        card.shape.setSize({cardW, cardH});
        card.shape.setFillColor(sf::Color(30, 40, 70, 240));
        card.shape.setOutlineColor(sf::Color(255, 215, 0));
        card.shape.setOutlineThickness(3);
        card.shape.setPosition({startX + i * (cardW + gap), startY});


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

            card.sprite.setPosition({
                card.shape.getPosition().x + (cardW - bounds.size.x * scale) / 2.0f,
                card.shape.getPosition().y + 10.0f
            });
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

    float startX = 765.0f - (totalW / 2.0f);
    float startY = 260.0f;

    currentOptions.reserve(count);

    for (int i = 0; i < count; ++i) {
        currentOptions.emplace_back(font, dummyTexture);
        CardOption& card = currentOptions.back();

        card.playerPtr = validCandidates[i]->clone();
        Player& pRef = *card.playerPtr;

        card.shape.setSize({cardW, cardH});
        card.shape.setFillColor(sf::Color(40, 40, 40, 240));
        card.shape.setOutlineColor(sf::Color::White);
        card.shape.setOutlineThickness(2);

        card.shape.setPosition({startX + i * (cardW + gap), startY});

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

            card.sprite.setPosition({
                card.shape.getPosition().x + (cardW - bounds.size.x * scale) / 2.0f,
                card.shape.getPosition().y + 10.0f
            });
        }

        card.nameText.setString(pRef.getName());
        card.nameText.setCharacterSize(14);
        sf::FloatRect textRect = card.nameText.getLocalBounds();
        card.nameText.setOrigin({textRect.size.x/2.0f, 0.0f});
        card.nameText.setPosition({card.shape.getPosition().x + cardW/2.0f, startY + cardH - 25.0f});

        card.ratingText.setString(to_string(pRef.getRating()));
        card.ratingText.setPosition({card.shape.getPosition().x + 5.0f, startY + 5.0f});
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

        float sidebarCenterX = 350.0f;
        float mY = 230.0f;

        sv.sprite.setScale({0.9f, 0.9f});
        sf::FloatRect b = sv.sprite.getLocalBounds();
        sv.sprite.setOrigin({b.size.x/2, b.size.y/2});
        sv.sprite.setPosition({sidebarCenterX, mY});

        sv.info.setString("Manager\n" + choice.manager.getNationality());
        sv.info.setCharacterSize(16);
        sv.info.setFillColor(sf::Color::Cyan);

        sf::FloatRect tr = sv.info.getLocalBounds();
        sv.info.setOrigin({tr.size.x/2, 0});
        sv.info.setPosition({sidebarCenterX, mY + 100.0f});

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

        sf::Vector2f pitchPos = formation.getCoordinates()[currentPositionIndex];

        sv.sprite.setScale({0.44f, 0.44f});

        sf::FloatRect bounds = sv.sprite.getLocalBounds();
        sv.sprite.setOrigin({bounds.size.x / 2.0f, bounds.size.y / 2.0f});
        sv.sprite.setPosition(pitchPos);

        sv.info.setString(posLabel + "\n" + to_string(rating));
        sv.info.setCharacterSize(12);
        sv.info.setOutlineColor(sf::Color::Black);
        sv.info.setOutlineThickness(1);
    }

    updateStatsUI();
    currentPositionIndex++;
    generateOptions();
}

void DraftSession::updateStatsUI() {
    int rating = static_cast<int>(team.computeRating());
    int chem = team.computeChemistry();
    int overall = static_cast<int>(team.computeOverall());

    ratingDisplay.setString("RATING: " + to_string(rating));
    chemistryDisplay.setString("CHEMISTRY: " + to_string(chem));
    overallDisplay.setString("OVERALL: " + to_string(overall));

    const auto& positions = formation.getPositions();
    const auto& coords = formation.getCoordinates();
    for (size_t i = 0; i < sidebarVisuals.size(); ++i) {
        if (i < positions.size()) {
            string posLabel = positions[i];

            const Player* pPtr = team.getPlayerOnPosition(posLabel);

            if (pPtr) {
                int indivChem = team.getPlayerChemistry(posLabel);

                string infoText = posLabel + " " + to_string(pPtr->getRating()) + "\nCh: " + to_string(indivChem);

                sidebarVisuals[i].info.setString(infoText);

                sf::FloatRect tr = sidebarVisuals[i].info.getLocalBounds();
                sidebarVisuals[i].info.setOrigin({tr.size.x / 2.0f, 0.0f});
                sidebarVisuals[i].info.setPosition({coords[i].x, coords[i].y + 35.0f});

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
                    sidebarVisuals[selectedSwapIndex].sprite.setScale({0.44f, 0.44f});
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
                    bool clickedOption = false;
                    for (size_t i = 0; i < currentOptions.size(); ++i) {
                        if (currentOptions[i].shape.getGlobalBounds().contains(mPos)) {
                            selectPlayer(static_cast<int>(i));
                            clickedOption = true;
                            break;
                        }
                    }
                    if(clickedOption) continue;
                }

                for (size_t i = 0; i < sidebarVisuals.size(); ++i) {
                    if (sidebarVisuals[i].sprite.getGlobalBounds().contains(mPos)) {
                        if (selectedSwapIndex == -1) {
                            selectedSwapIndex = static_cast<int>(i);
                            sidebarVisuals[i].sprite.setScale({0.48f, 0.48f});
                            sidebarVisuals[i].sprite.setColor(sf::Color(200, 255, 200));
                            cout << "Selectat pentru swap: Index " << i << "\n";
                        }
                        else if (selectedSwapIndex == static_cast<int>(i)) {
                            sidebarVisuals[i].sprite.setScale({0.44f, 0.44f});
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

                                sidebarVisuals[selectedSwapIndex].sprite.setPosition(formation.getCoordinates()[selectedSwapIndex]);
                                sidebarVisuals[i].sprite.setPosition(formation.getCoordinates()[i]);

                                updateStatsUI();
                                cout << "Swap reusit!\n";

                            } catch (const InvalidOperationException& e) {
                                cerr << "SWAP ERROR: " << e.what() << "\n";
                            }

                            for(auto& sv : sidebarVisuals) {
                                sv.sprite.setScale({0.44f, 0.44f});
                                sv.sprite.setColor(sf::Color::White);
                            }
                            if (!sidebarVisuals.empty() && sidebarVisuals.size() > formation.getPositions().size()) {
                                sidebarVisuals.back().sprite.setScale({0.2f, 0.2f});
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

    for (const auto& item : sidebarVisuals) {
        window.draw(item.sprite);
        window.draw(item.info);
    }

    window.draw(statsBackground);
    window.draw(ratingDisplay);
    window.draw(chemistryDisplay);
    window.draw(overallDisplay);
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

        pickText.setOutlineColor(sf::Color::Black);
        pickText.setOutlineThickness(2);

        sf::FloatRect tr = pickText.getLocalBounds();
        pickText.setOrigin({tr.size.x/2.0f, 0.0f});
        pickText.setPosition({765.0f, 50.0f});
        window.draw(pickText);

        sf::Vector2f mPos = window.mapPixelToCoords(sf::Mouse::getPosition(window));
        bool showPreview = false;

        if (!currentOptions.empty()) {
            sf::RectangleShape overlayBar({1280.0f, 250.0f});
            overlayBar.setFillColor(sf::Color(0,0,0,150));
            overlayBar.setPosition({0.0f, 240.0f});
            window.draw(overlayBar);
        }

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
                    previewSprite.setPosition({765.0f, 360.0f});

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
        sf::Text doneText(font, "ECHIPA COMPLETA", 30);
        doneText.setFillColor(sf::Color::Green);
        doneText.setOutlineColor(sf::Color::Black);
        doneText.setOutlineThickness(2);

        sf::FloatRect dBounds = doneText.getLocalBounds();
        doneText.setOrigin({dBounds.size.x, dBounds.size.y});
        doneText.setPosition({1260.0f, 700.0f});
        window.draw(doneText);
    }

    window.display();
}