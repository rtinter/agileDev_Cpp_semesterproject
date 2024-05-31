#ifndef ATHENA_GAMESESSION_HPP
#define ATHENA_GAMESESSION_HPP

#include <string>
#include <chrono>
#include <utility>
#include <vector>
#include "GameRunThrough.h"

/**
 * @brief Class for holding game session information.
 *
 * This class holds information about a game session.
 */
class GameSession {

    // General information
    int _gameID;
    int _userID;

    // Session information
    size_t _gameSessionUID;
    std::chrono::steady_clock::time_point _startPoint;
    std::chrono::steady_clock::time_point _endPoint;
    bool _ended;

    int _runThroughCount {1};

    /**
     * @brief Calculates the game session UID.
     *
     * This method calculates a unique identifier for the game session.
     *
     * @return The game session UID.
     */
    static size_t calcGameSessionUID();

    void increaseRunThroughCount();

public:

    std::vector<GameRunThrough> _gameRunThroughs;

    GameSession(int gameID, int userID);

    /**
     * @brief Save the current time as the end time of the game session.
     *
     * This method saves the current time as the end time of the game session.
     */
    void end();

    /**
     * @brief Get the duration of the game session in seconds.
     *
     * This method calculates the duration of the game session in seconds.
     * If the game session is still running, the current time is used as the end time.
     *
     * @return The duration of the game session in seconds.
     */
    unsigned long long getDurationInSeconds() const;

    /**
     * @brief Adds a new RunThrough of a game to the vector of Runthroughs of the current session.
     *
     * This method creates a new GameRunThrough that happened during the current Gamesession and adds it to the vector of Runthroughs.
     *
     * @param gameSessionUID the UID of the current gameSession (*this)
     * @param resultUnit the unit in which the result of the game is saved
     * @param result the result of the specific GameRunThrough
     */
    void addNewGameRunThrough(size_t const &gameSessionUID, std::string const &resultUnit, long const &result);

    int getGameID() const {
        return _gameID;
    }
    int getUserID() const {
        return _userID;
    }
    size_t getGameSessionUID() const {
        return _gameSessionUID;
    }
    std::chrono::steady_clock::time_point getStartPoint() const {
        return _startPoint;
    }
    std::chrono::steady_clock::time_point getEndPoint() const {
        return _endPoint;
    }
    bool isEnded() const {
        return _ended;
    }
};

#endif //ATHENA_GAMESESSION_HPP
