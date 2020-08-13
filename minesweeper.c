#include "minesweeper.h"
#include <stddef.h>
#include <stdbool.h>

/* References

For assistance in understanding calculation of indexes for row major order:
http://www.cbseguy.com/row-column-major-address-calculations-cbse/

*/

int coordinates_to_index(int dim, int* dim_max, int* coords) {
  int index = 0;
  for(int j = 0; j < dim; j++) {
    /*the first coordinate remains unchanged, as multiplied by one all others
    are multiplied by the size of the dimension proceeding it, i.e. the offset*/
    index += coords[j] * dim_max[j];
  }
  return index;
}

void index_to_coordinates(int index, int dim, int* dim_max, int* coords) {
  int index_value = index;
  for(int j = dim - 1; j > 0; j--) {
    coords[j] = (index_value/dim_max[j]);
    /*inverse operation to get index from coordinates,
     divide instead of multiply by the offset*/
    index_value = index_value % dim_max[j];
    //the remainder from the division process used to determine next coordinate
  }
  coords[0] = index_value;
  //return to base case, the 1D coordainate is always itself
}

int calculate_num_cells(int dim, int* dim_sizes) {
  int num_cells = 1;
  for(int j = 0; j < dim; j++) {
    num_cells = num_cells * dim_sizes[j];
  }
  return num_cells;
}

void calculate_size_of_dimensions(int dim, int* dim_sizes, int* maximums) {
  maximums[0] = 1;
  //initialise first element to one as 1D coordinates are unchanged in all cases
  for(int i = 1; i < dim; i++) {
    /*each new dimension will need to start at the end of the one before it,
    this array will store the appropriate offset for each dimension*/
    maximums[i] = dim_sizes[i-1] * maximums[i-1];
  }
}

bool is_adjacent(int* this_cell, int dim, int* other_cell) {
  int num_matched_coordinates = 0;
  for(int j = 0; j < dim; j++) {
    int test = this_cell[j] - other_cell[j];
    if(test == -1 || test == 0 || test == 1) {
      num_matched_coordinates++;
    }
  }
  if(num_matched_coordinates == dim) {
    return true;
  }
  else {
    return false;
  }
}

bool check_valid_coordinates(int dim, int* dim_sizes, int* coords) {
  for(int i = 0; i < dim; i ++) {
    if(coords[i] < 0 || coords[i] >= dim_sizes[i]) {
      return false;
    }
  }
  return true;
}

void recursive_select(struct cell* cell) {
  if(cell->hint == 0) {
    for(int i = 0; i < cell->num_adjacent; i++) {
      if(cell->adjacent[i]->selected == 0) {
        cell->adjacent[i]->selected = 1;
        recursive_select(cell->adjacent[i]);
      }
    }
  }
  else {
    return;
  }
}

bool check_win(struct cell* game, int num_cells) {
  for(int i = 0; i < num_cells; i++) {
    if(game[i].mined == 0 && game[i].selected == 0) {
      return false;
    }
  }
  return true;
}

void init_game(struct cell * game, int dim, int * dim_sizes, int num_mines, int ** mined_cells) {

  if(game == NULL || dim_sizes == NULL || mined_cells == NULL || dim < 0) {
    //check for invalid data
    return;
  }

  int num_cells = 0;
  num_cells = calculate_num_cells(dim, dim_sizes);

  int dim_max[MAX_DIM];
  calculate_size_of_dimensions(dim, dim_sizes, dim_max);

  for(int i = 0; i < num_cells; i++) {
    for(int k = 0; k < num_mines; k++) {
      int mine_index = coordinates_to_index(dim, dim_max, mined_cells[k]);
      if(mine_index == i) {
        game[i].mined = 1;
        break;
      }
      else {
        game[i].mined = 0;
      }
    }
    game[i].selected = 0;
    game[i].num_adjacent = 0;
    index_to_coordinates(i, dim, dim_max, game[i].coords);
    game[i].hint = 0;
  }

  //once all cells initialised go back and find adjacents by coordinates
  for(int n = 0; n < num_cells; n++) {
    for(int m = 0; m < num_cells; m++) {
      if(m != n) {
        //don't want to compare the cell against itself
        if(is_adjacent(game[n].coords, dim, game[m].coords)) {
          game[n].adjacent[game[n].num_adjacent] = &(game[m]);
          //append cell to end, so cells are stored in order
          game[n].num_adjacent++;
          if(game[m].mined == 1) {
            game[n].hint++;
          }
        }
      }
    }
  }
    return;
}

int select_cell(struct cell * game, int dim, int * dim_sizes, int * coords) {
    if(game == NULL || coords == NULL || dim_sizes == NULL || dim < 0) {
      return 0;
    }

    if(check_valid_coordinates(dim, dim_sizes, coords) == false) {
      return 0;
    }

    int dim_max[MAX_DIM];
    calculate_size_of_dimensions(dim, dim_sizes, dim_max);

    int index = coordinates_to_index(dim, dim_max, coords);

    if(game[index].selected == 1) {
      return 0;
    }

    if(game[index].mined == 1) {
      game[index].selected = 1;
      return 1;
    }

    game[index].selected = 1;
    recursive_select(&game[index]);

    int num_cells = 0;
    num_cells = calculate_num_cells(dim, dim_sizes);

    if(check_win(game, num_cells)) {
      return 2;
    }
    else {
      return 0;
    }
}
