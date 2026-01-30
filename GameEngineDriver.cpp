#include "GameEngine.h"

// This function will ask for a user input + process commands
void testGameStates(std::shared_ptr<GameEngine> engine) {

    while (true) {

        // Displays the current state to the user
        std::cout << "Current state: " << engine->GetCurrentStateName() << std::endl;

        std::string userInput;
        std::cout << "Enter a command: ";
        std::cin >> userInput;

        // Checks if the user is in the "win" state and wants to end the game
        if (engine->GetCurrentStateName() == "win" && userInput == "end") {
            
            // User entered "end" during the "win" state, so end the game
            std::cout << "Game ended." << std::endl;
            break;
        }

        // Processes the user input
        engine->ProcessCommand(userInput);
    }
}


// This function asks for user input to play the gamePlayLoop
void testMainGameLoop(){

    // create game engine
    GameEngine* engine = new GameEngine();
    // creating onjects to initialize game engine
    Deck* deck = new Deck(20);
    Hand* hand1 = new Hand(5);
    Hand* hand2 = new Hand(5);
    // filling hands
    for(int i = 0; i < 5; i++){
        std::cout << i << " card(s) drawn" << std::endl;
        deck->draw(hand1);
        deck->draw(hand2);
    }
    // creating player objects
    Player* p1 = new Player();
    Player* p2 = new Player();
    p1->setName("p1");
    p2->setName("p2");
    p1->setHand(hand1);
    p2->setHand(hand2);
    OrdersList* o1 = new OrdersList();
    OrdersList* o2 = new OrdersList();
    p1->setOrderList(o1);
    p2->setOrderList(o2);
    // creating territories and assigning them to each player
    vector<Territory*> list1;
    vector<Territory*> list2;
    vector<Territory*> allTer;
    allTer.push_back(new Territory(0, "France", "Europe", p1, 2));
    allTer.push_back(new Territory(1, "Belgium", "Europe", p1, 2));
    allTer.push_back(new Territory(2, "China", "Asia", p2, 2));
    list1.push_back(new Territory(0, "France", "Europe", p1, 2));
    list1.push_back(new Territory(1, "Belgium", "Europe", p1, 2));
    list2.push_back(new Territory(2, "China", "Asia", p2, 2));
    vector<string> continents;
    continents.push_back("Europe");
    continents.push_back("Asia");
    vector<int> border1;
    vector<int> border2;
    vector<int> border3;
    border1.push_back(0);
    border1.push_back(0);
    border1.push_back(1);
    border2.push_back(0);
    border2.push_back(0);
    border2.push_back(1);
    border3.push_back(1);
    border3.push_back(1);
    border3.push_back(0);
    vector<vector<int>> borders;
    borders.push_back(border1);
    borders.push_back(border2);
    borders.push_back(border3);
    Map* map = new Map("test", allTer, continents, borders);
    p1->setTerritoryList(list1);
    p2->setTerritoryList(list2);
    // create list of players
    vector<Player*> players;
    players.push_back(p1);
    players.push_back(p2);
    //setting the game engine
    engine->setDeck(deck);
    engine->setNumPlayer(2);
    engine->setPlayers(players);
    engine->setMap(map);

    //reinforcement phase
    engine->reinforcementPhase();

    //issueOrderPhase();
    engine->issueOrdersPhase();

    //executeOrdersPhase();
    engine->executeOrdersPhase();

}
/*
int main() {
    // Create GameEngine instance
    auto engine = std::make_shared<GameEngine>();

    // Create and add game states
    auto startState = std::make_shared<State>("start");
    auto mapLoadedState = std::make_shared<State>("map loaded");
    auto mapValidatedState = std::make_shared<State>("map validated");
    auto playersAddedState = std::make_shared<State>("players added");
    auto assignReinforcementState = std::make_shared<State>("assign reinforcement");
    auto issueOrdersState = std::make_shared<State>("issue orders");
    auto executeOrdersState = std::make_shared<State>("execute orders");
    auto winState = std::make_shared<State>("win");

    // Define transitions between states
    startState->AddTransition("loadmap", mapLoadedState);
    mapLoadedState->AddTransition("loadmap", mapLoadedState);
    mapLoadedState->AddTransition("validatemap", mapValidatedState);
    mapValidatedState->AddTransition("addplayer", playersAddedState);
    playersAddedState->AddTransition("addplayer", playersAddedState);
    playersAddedState->AddTransition("assigncountries", assignReinforcementState);
    assignReinforcementState->AddTransition("issueorder", issueOrdersState);
    issueOrdersState->AddTransition("issueorder", issueOrdersState);
    issueOrdersState->AddTransition("endissueorders", executeOrdersState);
    executeOrdersState->AddTransition("execorder", executeOrdersState);
    executeOrdersState->AddTransition("endexecorders", assignReinforcementState);
    executeOrdersState->AddTransition("win", winState);
    winState->AddTransition("play", startState);

    // Add states to the engine
    engine->AddState(startState);
    engine->AddState(mapLoadedState);
    engine->AddState(mapValidatedState);
    engine->AddState(playersAddedState);
    engine->AddState(assignReinforcementState);
    engine->AddState(issueOrdersState);
    engine->AddState(executeOrdersState);
    engine->AddState(winState);

    // Set the initial state
    engine->SetInitialState(startState);

    // Start the game
    engine->StartGame();

    // Call the testGameStates function to begin the game loop
    testGameStates(engine);

    return 0;
}
*/