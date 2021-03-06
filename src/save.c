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
 */
#include "../header/save.h"

char* get_save_file_path(char* maze_name) {
  char* file_name;
  int index;
  int size_file;

  size_file = strlen(maze_name) + strlen(SAVE_EXT);
  file_name = (char*)malloc(strlen(SAVE_FOLDER) + size_file + 1);

  if (file_name == NULL) {
    perror(RED "Malloc error save_maze");
    exit(1);
  }

  for (index = 0; index < forbidden_char_length; index++) {
    replace_char(maze_name, forbidden_char[index], '_');
  }

  strcpy(file_name, SAVE_FOLDER);
  strcat(file_name, maze_name);
  strcat(file_name, SAVE_EXT);

  return file_name;
}

void save_maze(maze_ maze, cell_** cells) {
  FILE* file;
  struct stat folder_stat;
  char* file_name;
  int line;
  int column;
  monster_ monster;

  if (stat(SAVE_FOLDER, &folder_stat) == -1) {
    mkdir(SAVE_FOLDER, 0700);
  }

  file_name = get_save_file_path(maze.name);
  file = fopen(file_name, "w");
  free(file_name);

  if (file == NULL) {
    fprintf(stderr, RED "Impossible to save the maze\n" RESET);
    wait_user_interaction();
    return;
  }

  fprintf(file, "%s\n", maze.name);
  fprintf(file, "%d\n", maze.height);
  fprintf(file, "%d\n", maze.width);
  fprintf(file, "%d\n", maze.monster_maze);
  fprintf(file, "%d\n", maze.difficulty);

  for (line = 0; line < maze.monster_maze; line++) {
    monster = maze.monster_list[line];
    fprintf(file, "%d\n", monster.line);
    fprintf(file, "%d\n", monster.init_line);
    fprintf(file, "%d\n", monster.column);
    fprintf(file, "%d\n", monster.init_column);
    fprintf(file, "%d\n", monster.tp_player_eaten);
    fprintf(file, "%d\n", monster.type);
  }

  for (line = 0; line < maze.height; line++) {
    for (column = 0; column < maze.width; column++) {
      fprintf(file, "%d\n", cells[line][column].number);
      fprintf(file, "%d\n", cells[line][column].score_value);
      fprintf(file, "%d\n", cells[line][column].symbol);
    }
  }

  fclose(file);
}

cell_** load_new_maze(maze_* maze) {
  char name_backup[NAME_MAZE_LENGTH];
  cell_** cells;
  strcpy(name_backup, maze->name);

  wprintf(L"\n");
  ask_maze_name(maze);
  cells = load_maze(maze);

  /* Rollback name if loading fail */
  if (cells == NULL) {
    strcpy(maze->name, name_backup);
  }

  return cells;
}

cell_** load_maze(maze_* maze) {
  FILE* file;
  cell_** cells;
  char* file_name;
  int line;
  int column;
  int value_tmp;
  monster_* monster;

  file_name = get_save_file_path(maze->name);
  file = fopen(file_name, "r");
  free(file_name);

  if (file == NULL) {
    fprintf(stderr, RED "This save file don't exist\n" RESET);
    return NULL;
  }

  fgets(maze->name, NAME_MAZE_LENGTH, file);
  strtok(maze->name, "\n");

  fscanf(file, "%d\n", &(maze->height));
  fscanf(file, "%d\n", &(maze->width));
  fscanf(file, "%d\n", &(maze->monster_maze));
  fscanf(file, "%d\n", &value_tmp);
  maze->difficulty = value_tmp;

  for (line = 0; line < maze->monster_maze; line++) {
    monster = &(maze->monster_list[line]);
    fscanf(file, "%d\n", &(monster->line));
    fscanf(file, "%d\n", &(monster->init_line));
    fscanf(file, "%d\n", &(monster->column));
    fscanf(file, "%d\n", &(monster->init_column));
    fscanf(file, "%d\n", &(monster->tp_player_eaten));
    fscanf(file, "%d\n", &(value_tmp));
    monster->type = value_tmp;

    set_parameters_monster(monster);
  }

  cells = allocte_cells_line(*maze);

  for (line = 0; line < maze->height; line++) {
    for (column = 0; column < maze->width; column++) {
      fscanf(file, "%d\n", &(cells[line][column].number));
      fscanf(file, "%d\n", &(cells[line][column].score_value));
      fscanf(file, "%d\n", &value_tmp);
      cells[line][column].symbol = value_tmp;
    }
  }

  fclose(file);

  wprintf(L"%sThe maze was loaded with success%s\n", GREEN, RESET);

  init_score(maze);

  return cells;
}

void delete_maze_save(maze_ maze) {
  char* file_name;
  file_name = get_save_file_path(maze.name);
  remove(file_name);
  free(file_name);
}

int show_save_files() {
  DIR* d;
  struct dirent* dir;
  char file_name[NAME_MAZE_LENGTH];
  char* file_name_pointer;
  int nb_saves;

  nb_saves = 0;
  d = opendir(SAVE_FOLDER);

  if (d) {
    while ((dir = readdir(d)) != NULL) {
      strcpy(file_name, dir->d_name);
      remove_substr(file_name, SAVE_EXT);

      file_name_pointer = get_save_file_path(file_name);

      if (is_regular_file(file_name_pointer)) {
        if (nb_saves == 0) {
          wprintf(L"%s\nAvailable save files :%s\n", GREEN, RESET);
        }

        nb_saves++;
        wprintf(L"  %s\n", file_name);
      }

      free(file_name_pointer);
    }

    closedir(d);
  }

  if (nb_saves == 0) {
    wprintf(L"%sYou don't have any save files%s\n", RED, RESET);
  }

  return nb_saves;
}

int is_regular_file(const char* path) {
  struct stat statbuf;
  return stat(path, &statbuf) == 0 && !S_ISDIR(statbuf.st_mode);
}

char* replace_char(char* str, char find, char replace) {
  char* current_pos;

  current_pos = strchr(str, find);
  while (current_pos) {
    *current_pos = replace;
    current_pos = strchr(current_pos, find);
  }

  return str;
}

void remove_substr(char* string, char* sub) {
  char* match;
  int len = strlen(sub);
  while ((match = strstr(string, sub))) {
    *match = '\0';
    strcat(string, match + len);
  }
}