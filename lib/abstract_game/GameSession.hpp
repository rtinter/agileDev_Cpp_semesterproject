#pragma once

#include <chrono>
#include <memory>
#include <string>
#include <vector>

#include "DataManager.hpp"
#include "GameIDs.hpp"
#include "GameRunThrough.hpp"

namespace abstract_game {
    /**
     * @brief Class for holding game session information.
     *
     * This class holds information about a game session.
     */
    class GameSession {
        // General information
        GameID _gameID;
        int _userID;

        // Session information
        size_t _gameSessionUID;

        time_t _begin;
        time_t _end;

        std::chrono::steady_clock::time_point _startPoint;
        std::chrono::steady_clock::time_point _endPoint;

        bool _ended;
        std::unique_ptr<DataManager> _dataManager;

        // Run through information
        int _runThroughCount{0};

        std::vector<GameRunThrough> _gameRunThroughs;

        /**
         * @brief Calculates the game session UID.
         *
         * This method calculates a unique identifier for the game session.
         *
         * @return The game session UID.
         */
        static size_t calcGameSessionUID();

        /**
         * @brief Increases the run through count.
         *
         * This method increases the run through count by one.
         */
        void increaseRunThroughCount();

        /**
         * @brief Writes the game session information to the data manager.
         *
         * This method hand over all game session information with all run-throughs to data manager which saves it.
         */
        void writeToDataManager() const;

    public:
        GameSession(GameID gameID, int userID);

        /**
         * @brief Save the current time as the end time of the game session.
         *
         * This method saves the current time as the end time of the game session.
         */
        void end();

        /**
         * @brief Adds a new RunThrough of a game to the vector of Runthroughs of the current session.
         *
         * This method creates a new GameRunThrough that happened during the current Gamesession and adds it to the vector of Runthroughs.
         *
         * @param resultUnit the unit in which the result of the game is saved
         * @param result the result of the specific GameRunThrough
         */
        void addNewGameRunThrough(std::string const &resultUnit, double const &result);
    };
} // abstract_game
