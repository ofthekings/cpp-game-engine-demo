#pragma once
#include <iostream>
#include <string>
#include <map>
#include <memory>   // Include the memory header for std::shared_ptr
#include "LoggingObserver.h"
#include "Player.h"
#include "Map.h"
#include "Cards.h"

// forward declaration
class State;
class Player;
class Map;
class Deck;

/*
*   The GameEngine controls the flow of the game.
*   It manages game states and state transitions.
*/
class GameEngine : public ILoggable, public Subject {

public:

    // Constructor
    GameEngine();

    // Copy constructor
    GameEngine(const GameEngine& other);

    // Destructor
    ~GameEngine();

    void AddState(std::shared_ptr<State> state);

    void SetInitialState(std::shared_ptr<State> initialState);

    void StartGame();

    void ProcessCommand(const std::string& command);

    void startupPhase();
    void reinforcementPhase();
    void issueOrdersPhase();
    void executeOrdersPhase();

    // getters and setters
    Deck* getDeck();
    void setDeck(Deck* d);
    int getNumPlayer();
    void setNumPlayer(int num);
    vector<Player*> getPlayers();
    void setPlayers(vector<Player*> p);
    Map* getMap();
    void setMap(Map* m);



    std::string GetCurrentStateName() const;

    // pointer to the current state:
    // declares a member variable called currentState 
    // that is a smart pointer capable of managing objects of the State class; 
    // it keeps track of the current state of the game
    std::shared_ptr<State> currentState;

    string stringToLog();

private:
    // map to store game states:
    // declares a member variable called states, which is used to store different 
    // game states associated with their names (strings) and is used in the game 
    // engine to manage and transition between states
    std::map<std::string, std::shared_ptr<State> > states;
    int numOfPlayer;
    Deck* deck;
    vector<Player*> playersVec;
    Map* map;
};


/*
 * State class represents a game state.
 * Each state has a name and can transition to other states based on commands.
 */
class State {
public:

    // Constructor
    State(const std::string& name);

    // Copy constructor
    State(const State& other);

    // Destructor
    ~State();

    void AddTransition(const std::string& command, std::shared_ptr<State> nextState);
    bool ProcessCommand(const std::string& command, GameEngine* engine);

    // Name of a state
    std::string stateName;

private:

    // Map to store transitions to other states:
    // member variable of the GameState class used to store a collection of 
    // transitions from the current game state to other game states. 
    std::map<std::string, std::shared_ptr<State> > transitions;
};