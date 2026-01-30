#include "GameEngine.h"
#include "Map.h"
#include <sstream>
#include <iostream>
#include <fstream>
#include <algorithm>
#include <ctime>
#include <random>


// Constructor: 
// initializes the current game state pointer to nullptr
GameEngine::GameEngine() : currentState(nullptr) {}

// Copy constructor implementation: 
// creates a deep copy of another GameEngine object
GameEngine::GameEngine(const GameEngine& other) {

    // Copy the current state
    if (other.currentState != nullptr) {

        // if the parameter "other" GameEngine has a valid current state, 
        // make a deep copy of it
        currentState = std::make_shared<State>(*(other.currentState));

    } else {

        // if the parameter "other" GameEngine has no current state, 
        // set the current state to nullptr
        currentState = nullptr;
    }

    // Copy the states
    for (const auto& pair : other.states) {

        // iterates through the other GameEngine's states 
        // and make deep copies of each state.
        states[pair.first] = std::make_shared<State>(*(pair.second));
    }
}

// Destructor implementation
GameEngine::~GameEngine() {
    // No need to manually delete with shared_ptr
    // because "shared_ptr" is a smart pointer that automatically 
    // manages objects so they will automatically be deleted
}

// Adds a new game state to the collection of states
void GameEngine::AddState(std::shared_ptr<State> state) {
    states[state->stateName] = state;
}

// Sets the initial game state when starting
void GameEngine::SetInitialState(std::shared_ptr<State> initialState) {
    currentState = initialState;
}

// Starts the game with the current state
void GameEngine::StartGame() {

    // If no initial state is set, display an error message and return
    if (currentState == nullptr) {
        std::cout << "No initial state set. Cannot start the game." << std::endl;
        return;
    }
}

// Processes a game command based on the current state
void GameEngine::ProcessCommand(const std::string& command) {

    // If no initial state is set, display an error message and return
    if (currentState == nullptr) {
        std::cout << "No initial state set. Cannot process commands." << std::endl;
        return;
    }

    if (currentState->ProcessCommand(command, this)) {
        // Successful command processing
        Notify(this);
    } else {
        // Invalid command for the current state
        std::cout << "Invalid command for the current state." << std::endl;
    }
}

// Get the name of the current game state
std::string GameEngine::GetCurrentStateName() const {

    // If a current state exists, return its name
    if (currentState != nullptr) {
        return currentState->stateName;
    }
    // No current existing state
    return "No current state";
}

// Initialize the startup components of the game
void GameEngine::startupPhase() {
    std::string command;        // holds user input
    bool mapLoaded = false;     // tracks if map is loaded
    Map* loadedMap = nullptr;   // pointer to loaded map; initially set to null
    bool mapValidated = false; //tracks if map is validated 
    bool playerAdded = false; //tracks if players are added
    bool gameStarted = false; //tracks is game is started

    while (!mapLoaded) {
        std::cout << "Enter loadmap <filename>: ";  // prompts the user to load the map
        std::cin >> command;                        // reads user input 

        if (command == "loadmap") {
            std::string filename;       // holds filename
            std::cin >> filename;       // reads filename 
            
            //TEST CODE 
            cout << "Map loaded" << endl; 
            mapLoaded = true;

            // loads the map using the Map class method
            //loadedMap = Map::readMapFile(filename);

            // checks if the map was loaded
            // if (loadedMap != nullptr) {
            //     mapLoaded = true;           // flag that map was loaded successfully

            //     // success
            //     std::cout << "Map '" << filename << "' loaded!." << std::endl;
            // } else {

            //     // loading failed
            //     std::cout << "Failed to load map '" << filename << "'." << std::endl;
            // }
        } else {

            // invalid command
            std::cout << "Invalid command. Please use 'loadmap <filename>' to load a map." << std::endl;
        }
    }

    while (!mapValidated) {
        std::cout << "Enter validatemap command: ";  // prompts the user to validate the map
        std::cin >> command;                        // reads user input 

        if (command == "validatemap") {
            //TEST CODE
            cout << "validate map" << endl;
            mapValidated = true;
        
            // if(loadedMap -> validate()) {
            //     std::cout << "Map validated" << std::endl;
            //     mapValidated = true;
            //}            
        }
        else {
            // invalid command
            std::cout << "Invalid command. Please use 'validatemap' to validate the selected map." << std::endl;
        }
    }

    while(!playerAdded) {
        std::cout << "Enter addplayer command: ";  // prompts the user to add players
        std::cin >> command;                        // reads user input 

        if (command == "addplayer") {
            cout << "adding player" << endl;
            
            // prompt user for number of players until a valid number is entered
            do {
                std::cout << "Enter a number between 2 and 6 (inclusive): ";
                std::cin >> numOfPlayer;

                // Check if the entered number is within the valid range
                if (numOfPlayer < 2 || numOfPlayer > 6) {
                    std::cout << "Invalid input! Please enter a number between 2 and 6.\n";
                }

            } while (numOfPlayer < 2 || numOfPlayer > 6);

            std::cout << "Loading Players..." << std::endl;

            // CREATE PLAYERS
            for(int i = 1; i <= numOfPlayer; i++) {
                std::string playerName = "Player " + std::to_string(i);
                std::cout << playerName << std::endl;

                //initialize player
                Player* p = new Player();
                p->setName(playerName);
                p->setHand(new Hand(10));
                
                //store players in a vector
                playersVec.push_back(p);
            }            
            playerAdded = true;        
        }
        else {
            // invalid command
            std::cout << "Invalid command. Please use 'addplayer' to load players." << std::endl;
        }
    }

    while (!gameStarted) {
        std::cout << "Enter gamestart command: ";  // prompts the user to start the game
        std::cin >> command;                        // reads user input 

        //create deck
        deck = new Deck(20);

        if (command == "gamestart") {
            cout << "game start" << endl;

            //equally distributing territories
            /* 
            int numTerritories = loadedMap->getTerritories().size() % playersVec.size();

            std::vector<int> distributedTerritories;

            for(const auto& player:playersVec) {
                int i = 0;
                while(i < numTerritories) {
                    int chosenTerritory = rand() % loadedMap->getTerritories().size();

                    if (!std::count(distributedTerritories.begin(), distributedTerritories.end(), chosenTerritory)) {
                        player->addTerritory(loadedMap->getTerritories().at(chosenTerritory));
                        distributedTerritories.push_back(chosenTerritory);
                        i++;
                    }
                }
            }
            */

            //randomize the positions by shuffling the vector where players are stored
            srand(time(NULL));
            std::shuffle(playersVec.begin(), playersVec.end(), std::random_device());

            //draw 2 cards & initialize 50 armies
            for (const auto& player : playersVec) {
                std::cout << player->getName() << " draws 2 cards " << "\n";
                 
                //draw 2 cards
                deck->draw(player->getHand());
                deck->draw(player->getHand());

                //assign 50 reinforcements
                std::cout << player->getName() << " assigned 50 armies " << "\n";
                player->setReinforcements(50);
                //deck draw
            }
            gameStarted = true;
        }
        else {
            // invalid command
            std::cout << "Invalid command. Please use 'gamestarted' to initialize the game." << std::endl;
        }

        //TODO: CALL MAIN GAME FUNCTION
    }
    // memory deallocation to prevent memory leaks
    //delete loadedMap;   
}


// reinforcementPhase(): assigns armies to each player
void GameEngine::reinforcementPhase(){
    vector<string> continentList = map->getContinents();

    for(Player* p : playersVec){
        vector<Territory*> playerTerritories = p->toDefend();
        // Default army size to distribute
        int armies = floor(playerTerritories.size()/3);

        // Checking if army size is smaller than the min value of 3
        if(armies < 3) armies = 3;

        // Checking if the player has all the territories of a given continent
        for(string cont : continentList){
            vector<Territory*> continentTerritories = map->getTerritoriesInContinent(cont);
            int allContinentTerritories = continentTerritories.size();
            int playerContinentTerritoryCount = 0;
            for(Territory* ter : continentTerritories){
                auto it = find(playerTerritories.begin(), playerTerritories.end(), ter);
                if(it != playerTerritories.end()){
                    playerContinentTerritoryCount++;
                }
                else{
                    break;
                }    
            }
            // Assigning bonus if the player has all the continents's territories
            if(allContinentTerritories == playerContinentTerritoryCount){
                armies += 5; //5 extra armies is the bonus per continent
            }
        }

        // Setting the reinforcements of the player
        p->setReinforcements(armies);
    }
}

// issueOrdersPhase(): allow players to create orders
void GameEngine::issueOrdersPhase(){
    // Loop through each player to create all their orders
    for(Player* p : playersVec){
        p->issueOrder(deck, playersVec);
    }
}

// executeOrdersPhase(): execute orders in a sequential manner with each play getting a turn per round
void GameEngine::executeOrdersPhase(){
    vector<int> orderListSizes;
    int longestList = 0;

    // Check the size of each player's order lists
    for(int i = 0; i < playersVec.size(); i++){
        int numOrders = playersVec[i]->getOrderList()->listOfOrders.size();
        if(numOrders > longestList) longestList = numOrders;
        orderListSizes.push_back(numOrders);
    }

    //Check that orders are greater than 1
    if(longestList <= 0){
        std::cout << "NOTE: Too few orders to execute!" << std::endl;
        return;
    }

    //Loop through orders
    for(int i = 0; i < longestList; i++){
        int playerIndex = 0;

        // Allow each player to execute one order per turn
        for(Player* p : playersVec){
            // Check the player has anymore orders
            if(orderListSizes[playerIndex] < i){
                std::cout << "Player " << p->getName() << "has no more orders. Next turn!" << std::endl;
                playerIndex++;
                continue;
            }
            else{
                // Executing the player's order
                p->getOrderList()->listOfOrders[0]->execute();

                //Removing the order from the beginning of the player's order list
                auto it = p->getOrderList()->listOfOrders.begin();
                delete p->getOrderList()->listOfOrders[0];
                p->getOrderList()->listOfOrders.erase(it);

                std::cout << "Player " << p->getName() << "is executing a " << p->getOrderList()->listOfOrders[0]->getOrderName() << "order" << std::endl;
            }
        }
    }
}


int GameEngine::getNumPlayer(){
    return numOfPlayer;
}
void GameEngine::setNumPlayer(int num){
    this->numOfPlayer = num;
}
Deck* GameEngine::getDeck(){
    return deck;
}
void GameEngine::setDeck(Deck* d){
    this->deck = d;
}
vector<Player*> GameEngine::getPlayers(){
    return this->playersVec;
}
void GameEngine::setPlayers(vector<Player*> p){
    this->playersVec = p;
}
Map* GameEngine::getMap(){
    return this->map;
}

void GameEngine::setMap(Map* m){
    this->map = m;
}

string GameEngine::stringToLog()
{
    if(currentState != nullptr){
        string str = "Game Engine Transitioned to State: ";
        str += currentState->stateName + ".";
        return str;
    }
    return "Current State is empty";
}

// Constructor: 
// initializes a game state with a given name
State::State(const std::string& name) : stateName(name) {}

// Copy constructor implementation:
// creates a deep copy of another state
State::State(const State& other) : stateName(other.stateName) {

    // Copy the transitions:
    // iterates through the other state's transitions 
    // and make copies of each transition
    for (const auto& pair : other.transitions) {
        transitions[pair.first] = std::make_shared<State>(*(pair.second));
    }
}

// Destructor implementation
State::~State() {
    // No need to manually delete transitions with shared_ptr
    // because "shared_ptr" is a smart pointer that automatically 
    // manages objects so they will automatically be deleted
}

// Adds a transition from the current state to another state based on a command
void State::AddTransition(const std::string& command, std::shared_ptr<State> nextState) {
    transitions[command] = nextState;
}

// Processes a command within the current state, allowing to transition to another state
bool State::ProcessCommand(const std::string& command, GameEngine* engine) {
    auto it = transitions.find(command);

    // if a transition exists for the given command, update the current state
    if (it != transitions.end()) {
        engine->currentState = it->second;
        return true;    // Valid command and state transition
        
    // if transition does not exist for the given command
    } else {
        return false;   // Invalid command for the current state
    }
}
