#include <iostream>
#include <string>

struct grid {
	int** cells;
	int rows;
	int cols;
	
	public:
		grid(int num_cols, std::string first_row) { // since we are adding rows, the number of rows for this constructor will always be 1, so using a variable is kinda redundant
			rows = 0;
			cols = num_cols;
			cells = new int*[1]; // cells is a 3D array, with one row to begin
			cells[0] = new int[num_cols]; // first row of cells is an integer array with length as the number of columns
			add_row(first_row);
		}

		void add_row(std::string rowToAdd) {
			
			int** newCells = new int*[rows + 1];
			for (int i = 0; i < rows; i++) {
				newCells[i] = new int[cols];
			}

			newCells[rows] = new int[cols];

			for (int i = 0; i < rows; i++) { // comparing i with number of rows BEFORE adding a new one
				for (int j = 0; j < cols; j++) {
					newCells[i][j] = cells[i][j];
				}
			}
			
			for (int j = 0; j < cols; j++) {
				newCells[rows][j] = rowToAdd[j] == 'O' ? 1 : 0;
			}

			for (int i = 0; i < rows; i++) { // free the old cells
				delete[] cells[i];
			}
			delete[] cells;

			rows += 1;
			cells = newCells;
		}
};

void printGrid(grid& grid) {
	// print pipes
	for (int i = 0; i < grid.cols; i++) {
		std::cout << "|";
	}
	std::cout << std::endl;

	// print grid
	for (int i = 0; i < grid.rows; i++) {
		for (int j  = 0; j < grid.cols; j++) {
			if (grid.cells[i][j] == 0) {
				std::cout << ".";
			} else if (grid.cells[i][j] == 1) {
				std::cout << "O";
			}
		}
		std::cout << std::endl;
	}

	// print pipes
	for (int i = 0; i < grid.cols; i++) {
		std::cout << "|";
	}
	std::cout << std::endl;
}

int countLiveNeighbors(grid& grid, int r, int c) {
	/*
		considering a non-edge cell (when r, c > 0) check each of
		cells[r-1][c-1], cells[r][c-1], cells[r+1][c-1], cells[r-1][c],
		cells[r+1][c], cells[r-1][c+1], cells[r][c+1], cells[r+1][c+1]
		[r-1] is up one row, [c-1] is left one column
		
		use a for loop to look thru 3x3 square, and skip the cell itself

		check that the current row and current column is not less than 0
		or greater than # of cols/rows, the cell would be on an edge or at 
		a corner, and we are trying to access a cell outside the grid

		if cell, live_neighbors += 1
	*/
	int live_neighbors = 0;
	for (int i = -1; i <= 1; i++) {
		for (int j = -1; j <= 1; j++) {
			if (i == 0 && j == 0) continue;
			int currentRow = r + i;
			int currentCol = c + j;
			if (currentRow >= 0 && currentCol >= 0 && currentRow < grid.rows && currentCol < grid.cols) {
				live_neighbors += grid.cells[currentRow][currentCol] ? 1 : 0;
			}
		}
	}	
	return live_neighbors;
}

void stepGrid(grid& grid) {
	/*
		logic:
			determine if the cell is dead:
			cell is dead:
				how many live neighbors?
				3 -> can be revived
				anything else -> still dead
			cell is alive
				how many live neighbors?
				2, 3 -> still alive
				anything else -> cell dies
		it would be helpful to have a live_neighbors function
		
		need an empty/temporary grid to make changes, so initial is not affected
	*/
	int** newCells = new int*[grid.rows];
	for (int i = 0; i < grid.rows; i++) {
		newCells[i] = new int[grid.cols];
		for (int j = 0; j < grid.cols; j++) {
			int liveNeighbors = countLiveNeighbors(grid, i, j);
			if (grid.cells[i][j] && (liveNeighbors == 2 || liveNeighbors == 3)) { // cell is alive, will remain alive
				newCells[i][j] = 1;
			} else if (!grid.cells[i][j] && liveNeighbors == 3) { // cell is dead, becomes alive
				newCells[i][j] = 1;
			} else { // otherwise, cell will either die (if alive) or remain dead
				newCells[i][j] = 0;
			}
		}
	}
	for (int i = 0; i < grid.rows; i++) {
		for (int j = 0; j < grid.cols; j++) {
			grid.cells[i][j] = newCells[i][j];
		}
		delete[] newCells[i];
	}
	delete[] newCells;
}

int main() {
	std::string line = " ";
	std::cin >> line;
	int numCols = line.length();
	grid grid(numCols, line);

	while (!std::cin.eof()) {
		std::cin >> line;
		if (line == "x") break;
		grid.add_row(line);
	}

	while (!std::cin.eof()) {
		char inputChar;
		std::cin >> inputChar;
		if (std::cin.eof()) break;
		if (inputChar == 's') {
			stepGrid(grid);
		} else if (inputChar == 'p') {
			printGrid(grid);
		}
	}

	return 0;
}