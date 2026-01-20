#include "DraftSession.h"
#include "UIFactory.h"
#include "Exception.h"
#include "Logger.h"
#include <algorithm>
#include <random>
#include <iostream>
#include <optional>
#include <iomanip>
#include <sstream>

using namespace std;

DraftSession::DraftSession(sf::RenderWindow &win, const Formation &f, SessionStats<int>& stats)
    : formation(f),
      team(f),
      db(),
      positionMap{
          {"GK", "GK"}, {"LB", "LB"}, {"LCB", "CB"}, {"RCB", "CB"}, {"RB", "RB"},
          {"LCM", "CM"}, {"CDM", "CM"}, {"RCM", "CM"}, {"LM", "LM"}, {"RM", "RM"},
          {"LW", "LW"}, {"RW", "RW"}, {"ST", "ST"}, {"LST", "ST"}, {"RST", "ST"},
          {"CB", "CB"}
      },
      state(DraftState::DRAFTING),
      globalStats(stats),
      window(win),
      font(),
      backgroundTexture(),
      backgroundSprite(backgroundTexture),
      defaultCardTexture(),
      defaultManagerTexture(),
      sidebar({250.0f, 720.0f}),
      dummyTexture(),
      ratingDisplay(font),
      chemistryDisplay(font),
      overallDisplay(font),
      statsBackground({250.0f, 100.0f}),
      currentPositionIndex(0),
      isDrafting(true),
      draftCompleted(false),
      choosingManager(false),
      currentOptions(),
      sidebarVisuals(),
      reserveVisuals(),
      previewSprite(dummyTexture),
      selectedSwapIndex(-1),
      linkLines(),
      finishButton({200.0f, 50.0f}),
      finishText(font),
      playMatchButton({250.0f, 60.0f}),
      playMatchText(font),
    summaryProcessed(false),
    soundPlayed(false)
{
    sidebarVisuals.reserve(12);
    reserveVisuals.reserve(7);

    if (!dummyTexture.resize({1, 1})) {}
    previewSprite.setTexture(dummyTexture);

    loadResources();
    generateOptions();

    finishButton = UIFactory::createButton({200.0f, 50.0f}, sf::Color::Green, {1150.0f, 650.0f});
    finishText = UIFactory::createText(font, "FINISH", 24, sf::Color::Black, {1150.0f, 650.0f});
    UIFactory::centerOrigin(finishText);

    playMatchButton = UIFactory::createButton({280.0f, 60.0f}, sf::Color(200, 50, 50), {640.0f, 400.0f});
    playMatchButton.setOutlineColor(sf::Color::White);
    playMatchButton.setOutlineThickness(2.0f);

    playMatchText = UIFactory::createText(font, "SIMULATE MATCH", 24, sf::Color::White, {640.0f, 400.0f});
    UIFactory::centerOrigin(playMatchText);
    analysisSystem = make_unique<AnalysisSystem>(font);
    SoundManager::getInstance().playDraftMusic();
}

void DraftSession::loadResources() {
    if (!font.openFromFile("arial.ttf")) throw FileMissingException("arial.ttf");

    ratingDisplay.setFont(font);
    chemistryDisplay.setFont(font);
    overallDisplay.setFont(font);

    db.loadAll();

    if (!defaultCardTexture.loadFromFile("images/players/card_default.png")) {
        if(!defaultCardTexture.resize({100, 100})) {}
    }
    defaultCardTexture.setSmooth(true);

    string path = "images/formations/" + formation.getName() + ".png";
    if (backgroundTexture.loadFromFile(path)) {
        backgroundTexture.setSmooth(true);
        backgroundSprite.setTexture(backgroundTexture, true);

        sf::FloatRect bgBounds = backgroundSprite.getLocalBounds();
        backgroundSprite.setOrigin({bgBounds.size.x / 2.0f, bgBounds.size.y / 2.0f});
        backgroundSprite.setPosition({765.0f, 360.0f});

        float scaleY = 720.0f / bgBounds.size.y;
        backgroundSprite.setScale({scaleY, scaleY});
    }

    sidebar = UIFactory::createButton({250.0f, 720.0f}, sf::Color(30, 30, 30), {125.0f, 360.0f});

    if (!defaultManagerTexture.loadFromFile("images/managers/manager_default.png")) {
        if(!defaultManagerTexture.resize({100, 100})) {}
    }
    defaultManagerTexture.setSmooth(true);


    statsBackground = UIFactory::createButton({260.0f, 120.0f}, sf::Color(0, 0, 0, 200), {365.f, 75.f});
    statsBackground.setOutlineColor(sf::Color::White);
    statsBackground.setOutlineThickness(2.0f);

    ratingDisplay = UIFactory::createText(font, "", 22, sf::Color::Yellow, {295.f, 30.f});
    ratingDisplay.setOrigin({0,0});

    chemistryDisplay = UIFactory::createText(font, "", 22, sf::Color::Cyan, {295.f, 65.f});
    chemistryDisplay.setOrigin({0,0});

    overallDisplay = UIFactory::createText(font, "", 22, sf::Color::Green, {295.f, 100.f});
    overallDisplay.setOrigin({0,0});

    updateStatsUI();
}

void DraftSession::generateManagerOptions() {
    currentOptions.clear();
    vector<Manager> candidates = db.getManagers();

    static random_device rd; static mt19937 g(rd());
    shuffle(candidates.begin(), candidates.end(), g);

    int count = min(static_cast<int>(candidates.size()), 5);
    float startX = 765.0f - ((count * 140.0f + (count - 1) * 20.0f) / 2.0f);

    for (int i = 0; i < count; ++i) {
        currentOptions.push_back(make_unique<CardOption>(font, dummyTexture));
        CardOption &card = *currentOptions.back();

        card.manager = candidates[i];
        card.isManager = true;

        card.shape = UIFactory::createButton({140.0f, 200.0f}, sf::Color(30, 40, 70, 240), {startX + i * 160.0f, 260.0f});
        card.shape.setOutlineColor(sf::Color(255, 215, 0));

        if (card.texture.loadFromFile(card.manager.getImagePath())) {
            card.texture.setSmooth(true);
            card.sprite.setTexture(card.texture, true);
        } else {
            card.sprite.setTexture(defaultManagerTexture, true);
        }

        sf::FloatRect b = card.sprite.getLocalBounds();
        if (b.size.x > 0 && b.size.y > 0) {
            float s = min(130.0f / b.size.x, 150.0f / b.size.y);
            card.sprite.setScale({s, s});
            card.sprite.setOrigin({b.size.x / 2.0f, b.size.y / 2.0f});
            card.sprite.setPosition(card.shape.getPosition());
        }

        card.nameText = UIFactory::createText(font, card.manager.getName(), 14, sf::Color::White, {card.shape.getPosition().x, card.shape.getPosition().y + 75});
        card.ratingText = UIFactory::createText(font, "MNG", 14, sf::Color::Cyan, {card.shape.getPosition().x - 65, card.shape.getPosition().y - 95});
    }
}

void DraftSession::generateOptions() {
    currentOptions.clear();
    const auto &pos = formation.getPositions();
    size_t starters = pos.size();
    size_t reservesCount = 7;
    size_t totalPlayers = starters + reservesCount;

    string dbGroup;

    if (currentPositionIndex < (int)starters) {
        dbGroup = positionMap[pos[currentPositionIndex]];
    }
    else if (currentPositionIndex < (int)totalPlayers) {
        int resIdx = currentPositionIndex - (int)starters;

        static random_device rd;
        static mt19937 g(rd());

        if (resIdx == 0) {
            dbGroup = "GK";
        } else if (resIdx == 1 || resIdx == 2) {
            vector<string> defs = {"CB", "LB", "RB"};
            uniform_int_distribution<> dist(0, 2);
            dbGroup = defs[dist(g)];
        } else if (resIdx == 3 || resIdx == 4) {
            vector<string> mids = {"CM", "LM", "RM"};
            uniform_int_distribution<> dist(0, 2);
            dbGroup = mids[dist(g)];
        } else {
            vector<string> atts = {"ST", "LW", "RW"};
            uniform_int_distribution<> dist(0, 2);
            dbGroup = atts[dist(g)];
        }
    }
    else {
        if (!choosingManager) { choosingManager = true; generateManagerOptions(); return; }
        else { draftCompleted = true; return; }
    }

    const auto &all = db.getPlayersByPosition(dbGroup);
    vector<Player*> valid;
    for(const auto& u : all) if(!team.isPlayerInTeam(*u)) valid.push_back(u.get());

    static random_device rd; static mt19937 g(rd());
    shuffle(valid.begin(), valid.end(), g);

    int count = min((int)valid.size(), 5);
    float startX = 765.0f - ((count * 140.0f + (count - 1) * 20.0f) / 2.0f);

    for(int i=0; i<count; ++i) {
        currentOptions.push_back(make_unique<CardOption>(font, dummyTexture));
        auto &card = *currentOptions.back();

        card.playerPtr = valid[i]->clone();

        card.shape = UIFactory::createButton({140.0f, 200.0f}, sf::Color(40,40,40,240), {startX + i*160.0f, 260.0f});

        bool loaded = false;
        if(!card.playerPtr->getImagePath().empty()) {
            if(card.texture.loadFromFile(card.playerPtr->getImagePath())) {
                loaded = true;
            }
        }

        if (loaded) {
            card.sprite.setTexture(card.texture, true);
        } else {
            card.sprite.setTexture(defaultCardTexture, true);
        }

        sf::FloatRect b = card.sprite.getLocalBounds();
        if (b.size.x > 0 && b.size.y > 0) {
            float s = min(130.0f / b.size.x, 150.0f / b.size.y);
            card.sprite.setScale({s, s});
            card.sprite.setOrigin({b.size.x / 2.0f, b.size.y / 2.0f});
            card.sprite.setPosition(card.shape.getPosition());
        }

        card.nameText = UIFactory::createText(font, card.playerPtr->getName(), 14, sf::Color::White, {card.shape.getPosition().x, card.shape.getPosition().y + 75});
        card.ratingText = UIFactory::createText(font, to_string(card.playerPtr->getRating()), 14, sf::Color::Yellow, {card.shape.getPosition().x - 65, card.shape.getPosition().y - 95});
    }
}

void DraftSession::selectPlayer(int index) {
    if (index < 0 || index >= (int)currentOptions.size()) return;
    auto &opt = *currentOptions[index];

    if (opt.isManager) {
        team.setManager(opt.manager);

        sf::Texture tex = (opt.texture.getSize().x > 0) ? opt.texture : defaultManagerTexture;
        sidebarVisuals.push_back(make_unique<SelectedVisual>(font, tex));
        auto &sv = *sidebarVisuals.back();
        sv.sprite.setTexture(sv.texture, true);
        sv.sprite.setPosition({350.0f, 230.0f});

        sf::FloatRect b = sv.sprite.getLocalBounds();
        sv.sprite.setOrigin({b.size.x / 2.0f, b.size.y / 2.0f});
        sv.sprite.setScale({0.8f, 0.8f});

        sv.info.setString(opt.manager.getName());
        sv.info.setCharacterSize(18);
        sv.info.setFillColor(sf::Color::White);
        sv.info.setOutlineColor(sf::Color::Black);
        sv.info.setOutlineThickness(2);

        sf::FloatRect tr = sv.info.getLocalBounds();
        sv.info.setOrigin({tr.size.x / 2.0f, 0.0f});
        sv.info.setPosition({350.0f, 320.0f});

        draftCompleted = true; choosingManager = false; currentOptions.clear();
        updateStatsUI();
        return;
    }

    if (opt.playerPtr) {
        sf::Texture tex = (opt.texture.getSize().x > 0) ? opt.texture : defaultCardTexture;

        if (currentPositionIndex < (int)formation.getPositions().size()) {
            team.addPlayer(formation.getPositions()[currentPositionIndex], std::move(opt.playerPtr));

            sidebarVisuals.push_back(make_unique<SelectedVisual>(font, tex));
            auto &sv = *sidebarVisuals.back();
            sv.sprite.setTexture(sv.texture, true);
            sv.sprite.setPosition(formation.getCoordinates()[currentPositionIndex]);

            sf::FloatRect b = sv.sprite.getLocalBounds();
            sv.sprite.setOrigin({b.size.x / 2.0f, b.size.y / 2.0f});
            sv.sprite.setScale({0.44f, 0.44f});

            sv.info.setString(formation.getPositions()[currentPositionIndex]);
            sv.info.setCharacterSize(14);
            sv.info.setOutlineColor(sf::Color::Black);
            sv.info.setOutlineThickness(1.5f);

            sf::Vector2f pos = sv.sprite.getPosition();
            sv.info.setPosition({pos.x, pos.y + 60.0f});

        } else {
            team.addReserve(std::move(opt.playerPtr));

            reserveVisuals.push_back(make_unique<SelectedVisual>(font, tex));
            auto &sv = *reserveVisuals.back();
            sv.sprite.setTexture(sv.texture, true);
            sv.sprite.setPosition({125.0f, 100.0f + (currentPositionIndex - (int)formation.getPositions().size()) * 85.0f});

            sf::FloatRect b = sv.sprite.getLocalBounds();
            sv.sprite.setOrigin({b.size.x / 2.0f, b.size.y / 2.0f});
            sv.sprite.setScale({0.3f, 0.3f});

            sv.info.setString("");
        }
    }
    updateStatsUI();
    currentPositionIndex++;
    generateOptions();
}

void DraftSession::updateStatsUI() {
    updateLinksVisuals();
    ratingDisplay.setString("RATING: " + to_string((int)team.computeRating()));
    chemistryDisplay.setString("CHEMISTRY: " + to_string(team.computeChemistry()));
    overallDisplay.setString("OVERALL: " + to_string((int)team.computeOverall()));

    const auto &positions = formation.getPositions();

    for (size_t i = 0; i < sidebarVisuals.size(); ++i) {
        if (i < positions.size()) {
            string posLabel = positions[i];
            const Player* pPtr = team.getPlayerOnPosition(posLabel);

            if (pPtr) {
                int indivChem = team.getPlayerChemistry(posLabel);
                string infoText = posLabel + "  Ch:" + to_string(indivChem);

                sidebarVisuals[i]->info.setString(infoText);
                sidebarVisuals[i]->info.setCharacterSize(14);
                sidebarVisuals[i]->info.setScale({1.0f, 1.0f});

                sf::FloatRect tr = sidebarVisuals[i]->info.getLocalBounds();
                sidebarVisuals[i]->info.setOrigin({tr.size.x / 2.0f, 0.0f});

                sf::Vector2f spritePos = sidebarVisuals[i]->sprite.getPosition();
                sidebarVisuals[i]->info.setPosition({spritePos.x, spritePos.y + 60.0f});

                if (indivChem == 10) sidebarVisuals[i]->info.setFillColor(sf::Color::Green);
                else if (indivChem >= 7) sidebarVisuals[i]->info.setFillColor(sf::Color::Yellow);
                else if (indivChem >= 4) sidebarVisuals[i]->info.setFillColor(sf::Color(255, 165, 0));
                else sidebarVisuals[i]->info.setFillColor(sf::Color::Red);
            } else {
                sidebarVisuals[i]->info.setString(posLabel);
                sidebarVisuals[i]->info.setFillColor(sf::Color::White);
                sidebarVisuals[i]->info.setCharacterSize(14);
                sf::FloatRect tr = sidebarVisuals[i]->info.getLocalBounds();
                sidebarVisuals[i]->info.setOrigin({tr.size.x / 2.0f, 0.0f});

                sf::Vector2f spritePos = sidebarVisuals[i]->sprite.getPosition();
                sidebarVisuals[i]->info.setPosition({spritePos.x, spritePos.y + 60.0f});
            }
        }
    }

    for (const auto& res : reserveVisuals) {
        res->info.setString("");
    }
}

void DraftSession::updateLinksVisuals() {
    linkLines.clear();
    const auto &links = formation.getLinks();
    const auto &coords = formation.getCoordinates();
    const auto &pos = formation.getPositions();

    for (const auto &link : links) {
        auto it1 = find(pos.begin(), pos.end(), link.first);
        auto it2 = find(pos.begin(), pos.end(), link.second);
        if (it1 != pos.end() && it2 != pos.end()) {
            int i1 = distance(pos.begin(), it1);
            int i2 = distance(pos.begin(), it2);

            const Player* p1 = team.getPlayerOnPosition(link.first);
            const Player* p2 = team.getPlayerOnPosition(link.second);
            int val = (p1 && p2) ? p1->calcLink(*p2) : 0;

            if (val == 3) linkLines.push_back(make_unique<GreenLink>(coords[i1], coords[i2]));
            else if (val == 2) linkLines.push_back(make_unique<YellowLink>(coords[i1], coords[i2]));
            else if (val == 1) linkLines.push_back(make_unique<OrangeLink>(coords[i1], coords[i2]));
            else linkLines.push_back(make_unique<RedLink>(coords[i1], coords[i2]));
        }
    }
}

void DraftSession::handleSwapSelection(int index, bool isReserve) {
    int encodedIndex = isReserve ? (100 + index) : index;

    if (selectedSwapIndex == -1) {
        selectedSwapIndex = encodedIndex;
        if (isReserve) {
            reserveVisuals[index]->sprite.setColor(sf::Color::Green);
            reserveVisuals[index]->sprite.setScale({0.35f, 0.35f});
        } else {
            sidebarVisuals[index]->sprite.setColor(sf::Color::Green);
            sidebarVisuals[index]->sprite.setScale({0.48f, 0.48f});
        }
        return;
    }

    if (selectedSwapIndex == encodedIndex) {
        if (isReserve) {
            reserveVisuals[index]->sprite.setColor(sf::Color::White);
            reserveVisuals[index]->sprite.setScale({0.30f, 0.30f});
        } else {
            sidebarVisuals[index]->sprite.setColor(sf::Color::White);
            sidebarVisuals[index]->sprite.setScale({0.44f, 0.44f});
        }
        selectedSwapIndex = -1;
        return;
    }

    try {
        bool firstIsReserve = (selectedSwapIndex >= 100);
        int idx1 = firstIsReserve ? (selectedSwapIndex - 100) : selectedSwapIndex;
        int idx2 = index;

        if (!firstIsReserve && !isReserve) {
            string pos1 = formation.getPositions()[idx1];
            string pos2 = formation.getPositions()[idx2];
            team.swapPlayers(pos1, pos2);

            swap(sidebarVisuals[idx1], sidebarVisuals[idx2]);

            sidebarVisuals[idx1]->sprite.setPosition(formation.getCoordinates()[idx1]);
            sidebarVisuals[idx2]->sprite.setPosition(formation.getCoordinates()[idx2]);

        } else if (!firstIsReserve && isReserve) {
            string pos = formation.getPositions()[idx1];
            team.swapStarterWithReserve(pos, idx2);

            swap(sidebarVisuals[idx1], reserveVisuals[idx2]);

            sidebarVisuals[idx1]->sprite.setPosition(formation.getCoordinates()[idx1]);
            float startY = 100.0f; float gapY = 85.0f;
            reserveVisuals[idx2]->sprite.setPosition({125.0f, startY + idx2 * gapY});

        } else if (firstIsReserve && !isReserve) {
            string pos = formation.getPositions()[idx2];
            team.swapStarterWithReserve(pos, idx1);

            swap(reserveVisuals[idx1], sidebarVisuals[idx2]);

            sidebarVisuals[idx2]->sprite.setPosition(formation.getCoordinates()[idx2]);
            float startY = 100.0f; float gapY = 85.0f;
            reserveVisuals[idx1]->sprite.setPosition({125.0f, startY + idx1 * gapY});

        } else {
            team.swapReserves(idx1, idx2);
            swap(reserveVisuals[idx1], reserveVisuals[idx2]);

            float startY = 100.0f; float gapY = 85.0f;
            reserveVisuals[idx1]->sprite.setPosition({125.0f, startY + idx1 * gapY});
            reserveVisuals[idx2]->sprite.setPosition({125.0f, startY + idx2 * gapY});
        }
        updateStatsUI();
    } catch (const GameException &e) {
        cerr << "SWAP BLOCAT: " << e.what() << endl;
    }

    for (size_t k = 0; k < sidebarVisuals.size(); ++k) {
        if (k >= formation.getPositions().size()) sidebarVisuals[k]->sprite.setScale({0.9f, 0.9f});
        else sidebarVisuals[k]->sprite.setScale({0.44f, 0.44f});
        sidebarVisuals[k]->sprite.setColor(sf::Color::White);
    }
    for (const auto &rv: reserveVisuals) {
        rv->sprite.setScale({0.30f, 0.30f});
        rv->sprite.setColor(sf::Color::White);
    }
    selectedSwapIndex = -1;
}

void DraftSession::handleInput() {
    while (const auto event = window.pollEvent()) {
        if (event->is<sf::Event::Closed>()) {
            window.close();
            isDrafting = false;
        }
        else if (const auto* kp = event->getIf<sf::Event::KeyPressed>()) {
             if (kp->code == sf::Keyboard::Key::Escape) {
                 if (selectedSwapIndex != -1) {
                     bool isRes = (selectedSwapIndex >= 100);
                     int idx = isRes ? (selectedSwapIndex - 100) : selectedSwapIndex;
                     if(isRes) reserveVisuals[idx]->sprite.setColor(sf::Color::White);
                     else sidebarVisuals[idx]->sprite.setColor(sf::Color::White);
                     selectedSwapIndex = -1;
                 } else {
                     isDrafting = false;
                 }
             }
        }
        else if (const auto* mp = event->getIf<sf::Event::MouseButtonPressed>()) {
            if (mp->button == sf::Mouse::Button::Left) {
                sf::Vector2f mPos = window.mapPixelToCoords(sf::Mouse::getPosition(window));

                if (state == DraftState::SUMMARY) {

                    if (playMatchButton.getGlobalBounds().contains(mPos)) {
                        state = DraftState::SIMULATION;
                        SoundManager::getInstance().stopDraftMusic();
                        SoundManager::getInstance().playMatchMusic();
                        matchSystem = make_unique<MatchSystem>(font, (int)team.computeOverall());
                        matchSystem->startMatch();
                        return;
                    }


                    if (!playMatchButton.getGlobalBounds().contains(mPos)) {
                        isDrafting = false;
                        return;
                    }
                }

                if (state == DraftState::SIMULATION) {
                    if (matchSystem && matchSystem->isFinished()) {
                        SoundManager::getInstance().stopEffects();
                        isDrafting = false;
                    }
                    return;
                }

                if (draftCompleted && finishButton.getGlobalBounds().contains(mPos)) {
                    state = DraftState::SUMMARY;
                    globalStats.addEntry((int)team.computeOverall());
                    return;
                }

                if (!draftCompleted) {
                    for(int i=0; i<(int)currentOptions.size(); ++i) {
                        if (currentOptions[i]->shape.getGlobalBounds().contains(mPos)) {
                            selectPlayer(i); return;
                        }
                    }
                }

                for(int i=0; i<(int)sidebarVisuals.size(); ++i) {
                    if(i < (int)formation.getPositions().size() && sidebarVisuals[i]->sprite.getGlobalBounds().contains(mPos)) {
                        handleSwapSelection(i, false); return;
                    }
                }
                for(int i=0; i<(int)reserveVisuals.size(); ++i) {
                    if(reserveVisuals[i]->sprite.getGlobalBounds().contains(mPos)) {
                        handleSwapSelection(i, true); return;
                    }
                }
            }
        }
    }
}

void DraftSession::drawSummary() {

    if (!summaryProcessed) {
        string logMsg = "Draft Finished. Final Score: " + to_string((int)team.computeOverall());
        Logger::getInstance().log(logMsg);

        if (analysisSystem) {
            analysisSystem->analyzeTeam(team, formation);
        }

        summaryProcessed = true;
    }

    window.clear(sf::Color(10, 10, 30));

    sf::Text title = UIFactory::createText(font, "DRAFT COMPLETE!", 50, sf::Color(255, 215, 0), {640, 100});
    UIFactory::centerOrigin(title);

    sf::Text score = UIFactory::createText(font, "Final Overall: " + to_string((int)team.computeOverall()), 40, sf::Color::White, {640, 200});
    UIFactory::centerOrigin(score);

    std::stringstream ss;
    ss << fixed << setprecision(1) << globalStats.getAverage();

    sf::Text statsText = UIFactory::createText(font,
        "Session Avg: " + ss.str() + " | Drafts Played: " + to_string(globalStats.getHistory().size()),
        28, sf::Color::Yellow, {640, 300});
    UIFactory::centerOrigin(statsText);

    if (analysisSystem) {
        analysisSystem->draw(window);
    }

    window.draw(playMatchButton);
    window.draw(playMatchText);

    sf::Text exitMsg = UIFactory::createText(font, "Click outside button to return to Menu", 20, sf::Color::Cyan, {640, 650});
    UIFactory::centerOrigin(exitMsg);

    window.draw(title);
    window.draw(score);
    window.draw(statsText);
    window.draw(exitMsg);
}

void DraftSession::draw() {
    if (state == DraftState::SUMMARY) {
        drawSummary();
        window.display();
        return;
    }

    if (state == DraftState::SIMULATION) {
        window.clear(sf::Color(10, 30, 10));
        if (matchSystem) {
            matchSystem->update();
            matchSystem->draw(window);
        }

        if (matchSystem && matchSystem->isFinished()) {
            SoundManager::getInstance().stopMatchMusic();
            if (!soundPlayed) {
                int uScore = matchSystem->getUserScore();
                int oScore = matchSystem->getOpponentScore();

                if (uScore > oScore) {
                    SoundManager::getInstance().playWinSound();
                } else {
                    SoundManager::getInstance().playDrawSound();
                }
                soundPlayed = true;
            }
             sf::Text over = UIFactory::createText(font, "Click to Exit", 20, sf::Color::White, {640, 600});
             UIFactory::centerOrigin(over);
             window.draw(over);
        }

        window.display();
        return;
    }

    window.clear(sf::Color(20, 20, 20));
    window.draw(backgroundSprite);
    window.draw(sidebar);

    for(const auto& l : linkLines) l->draw(window);
    for(const auto& s : sidebarVisuals) { window.draw(s->sprite); window.draw(s->info); }
    for(const auto& r : reserveVisuals) { window.draw(r->sprite); window.draw(r->info); }

    window.draw(statsBackground);
    window.draw(ratingDisplay);
    window.draw(chemistryDisplay);
    window.draw(overallDisplay);

    if (draftCompleted) {
        window.draw(finishButton);
        window.draw(finishText);
    } else {
        for(const auto& opt : currentOptions) {
            window.draw(opt->shape);
            window.draw(opt->sprite);
            window.draw(opt->nameText);
            window.draw(opt->ratingText);
        }
    }

    window.display();
}

void DraftSession::run() {
    while (isDrafting && window.isOpen()) {
        handleInput();
        draw();
    }
}