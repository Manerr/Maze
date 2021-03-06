/**
 * ENSICAEN
 * 6 Boulevard Maréchal Juin
 * F-14050 Caen Cedex
 *
 * This file is owned by ENSICAEN students. No portion of this
 * document may be reproduced, copied or revised without written
 * permission of the authors.
 *
 * @author Antonin Huaut <antonin.huaut@ecole.ensicaen.fr>
 * @version 1.0.0 - 2020-09-30
 *
 * \file player.h
 * \brief Management of user and test entries
 */
#ifndef PLAYER_H
#define PLAYER_H

#include <unistd.h>
#include "./display.h"

/**
 * \brief Tp player to maze entrance
 * \param player player struct
 */
void tp_player_entrance(player_* player);

/**
 * \brief Call when a player is eaten by a monster
 * \param maze maze structure
 * \param cells cells structure array of the maze
 * \param player player structure address
 */
void eat_player(maze_ maze, cell_** cells, player_* player);

/**
 * \brief Check if the player is being eaten by the monster
 * \param player player struct
 * \param maze maze structure
 * \return int 1 if eaten, else 0
 */
int check_player_on_monster(player_* player, maze_ maze);

/**
 * \brief Check if the player can move in the requested direction
 * \param movement movement wchar_t value
 * \param player player structure address
 * \param maze maze structure
 * \param cells cells structure array of the maze
 * \return int 1 if the player can move, else 0
 */
int can_move(wchar_t movement, player_* player, maze_ maze, cell_** cells);

/**
 * \brief Check if the movement char is valid
 * \param movement movement wchar_t value
 * \return int 1 if valid, else 0
 */
int is_valid_movement_char(wchar_t movement);

/**
 * \brief Get the specified cell if it's not a wall
 * \param line line number
 * \param column column number
 * \param maze maze structure
 * \param cells cells structure array of the maze
 * \return cell_* cell structure
 */
cell_* get_empty_cell(int line, int column, maze_ maze, cell_** cells);

/**
 * \brief Ask to the user to do a movement
 * \return int movement char value
 */
int ask_movement();

/**
 * \brief Calculate player score
 * \param player player structure
 * \return player score
 */
int get_player_score(player_ player);

/**
 * \brief Ask to the user the maze options
 * \param maze maze structure address
 * \return cell_** cells structure array of the maze
 */
cell_** ask_maze_options(maze_* maze);

/**
 * \brief Ask to the user the maze name
 * \param maze maze structure address
 */
void ask_maze_name(maze_* maze);

#endif