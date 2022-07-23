using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace KnightsTour
{
    class Program
    {
        //Values used when setting up the grid starting the tour
        int boardWidth;
        int boardHeight;
        int startPosX;
        int startPosY;

        int moveAttempts = 0; //Used to count how many attempts have been made so that the user can stay informed on how fast the solver is running 

        int[,] board; //This is where the order of the knights movement is stored, it is also checked during the solving to know where the knight can still move

        //a 2D array of possible moves for the knight
        public int[,] moves = new int[,] 
        {
            { 1, -2 },
            { 2, -1 },
            { 2, 1 },
            { 1, 2 },
            { -1, 2 },
            { -2, 1 },
            { -2, -1 },
            { -1, -2 }
        };

        static void Main(string[] args)
        {
            Console.WriteLine("Knights Tour Solver created by Elliot Chester");
            var tour = new Program();
            tour.RecieveInput();
            tour.SolveTour();
            Console.ReadKey();
        }

        //Where all of the user input takes place, the solver needs the width and height of the grid, aswell as the x and y start positions
        public void RecieveInput()
        {
            bool parseSuccess = false; //used to check if the user has made a mistake so that they can be asked to reenter the input
            Console.WriteLine("\nInput Grid Width");
            while (!parseSuccess) //repeats this step until the user has inputted the correct format
            {
                parseSuccess = int.TryParse(Console.ReadLine(), out boardWidth);
                if (!parseSuccess) Console.WriteLine("Input must be an int"); //if parseSuccess is false, it means that the user has inputted text that isnt an int, so they are asked to reenter it
            }

            parseSuccess = false;
            Console.WriteLine("\nInput Grid Height");
            while (!parseSuccess)
            {
                parseSuccess = int.TryParse(Console.ReadLine(), out boardHeight);
                if (!parseSuccess) Console.WriteLine("Input must be an int");
            }

            parseSuccess = false;
            Console.WriteLine("\nInput Start X");
            while (!parseSuccess || startPosX < 0 || startPosX >= boardWidth) //repeats until the users input is the correct format and within the bounds of the grid
            {
                parseSuccess = int.TryParse(Console.ReadLine(), out startPosX);
                if (!parseSuccess)
                {
                    Console.WriteLine("Input must be an int");
                }
                else
                {   //tells the user what to fix based on their incorrect input
                    if (startPosX < 0) Console.WriteLine("Input cannot be below 0");
                    if (startPosX >= boardWidth) Console.WriteLine($"Input cannot be below more than {boardWidth - 1} (board width - 1)");
                }
            }

            parseSuccess = false;
            Console.WriteLine("\nInput Start Y");
            while (!parseSuccess || startPosY < 0 || startPosY >= boardHeight)
            {
                parseSuccess = int.TryParse(Console.ReadLine(), out startPosY);
                if (!parseSuccess)
                {
                    Console.WriteLine("Input must be an int");
                }
                else
                {
                    if (startPosY < 0) Console.WriteLine("Input cannot be below 0");
                    if (startPosY >= boardHeight) Console.WriteLine($"Input cannot be below more than {boardHeight}(board height -1)");
                }
            }
        }

        //Where all of the Solving is controlled
        public void SolveTour()
        {

            SetUpBoard(); //Sets up the board using the inputs from above 

            Console.WriteLine($"\nAttempting to find a solution for a {boardWidth} by {boardHeight} grid, starting at {startPosX}, {startPosY}");

            board[startPosX, startPosY] = 0;
            if (!GetNextMove(startPosX, startPosY, 1))//this is what starts the search, if it returns false, it means that there was no tour found
            {
                Console.WriteLine($"{moveAttempts} attempted moves were made, no solution was found");
                return; //returns so that the grids are not drawn
            }

            Console.WriteLine($"\nTour Complete\nIt took {moveAttempts} move attempts to find a solution");

            DrawCompletedGrid(); //Draws a static grid with the move numbers

            Task.Run(DrawTour); //Draws a dynamic grid that updates regularly to show the order of movement
        }

        //Sets up the board for use when solving
        void SetUpBoard()
        {
            board = new int[boardWidth, boardHeight]; //creates a 2D array that represents the chess board

            //sets every int in the array to -1
            for (int i = 0; i < boardWidth; i++)
            {
                for (int j = 0; j < boardHeight; j++)
                {
                    board[i, j] = -1;
                }
            }
        }

        //Controls the search for the next position the knight can move to, takes the position of the cell to check, and how many moves have been made to get to this point
        bool GetNextMove(int x, int y, int currentMove)
        {
            moveAttempts++;
            if(moveAttempts % 100000000 == 0) Console.WriteLine(moveAttempts.ToString() + " Attempted Moves");

            //If the current move is the same as the total amount of cells, the tour is complete
            if (currentMove >= boardWidth * boardHeight)
            {
                return true;
            }

            //loops though each of the posible moves
            for (int i = 0; i < 8; i++)
            {
                int checkX = x + moves[i, 0]; //sets the X and Y positions of the next cell to check by adding the current cell to the move currently being checked
                int checkY = y + moves[i, 1]; //

                if (checkX < 0 || checkX >= boardWidth || checkY < 0 || checkY >= boardHeight || board[checkX, checkY] != -1) continue; //checks if the check is out of bounds, or already checked, if it is, this check is skipped

                board[checkX, checkY] = currentMove; //sets the position being checked to the current move to show what order the knight moves, aswell as to stop it from being checked in the future
                if (GetNextMove(checkX, checkY, currentMove + 1))
                {
                    return true; //if this comes back as true, it means the solution has been found with that cell so the tour can complete
                }
                else
                {
                    board[checkX, checkY] = -1; //if the check returned false, the cell we checked is returned to -1 so that it can be checked later
                }
            }
            return false;
        }

        //Used to draw a static grid when the tour has been completed
        public void DrawCompletedGrid()
        {
            string debugString = "\nCompleted Grid"; //a string is used here so that the grid can be drawn in 1 console.writeline

            debugString += FillLine(boardWidth);//writes the of the grid

           
            for (int i = 0; i < boardHeight; i++) //loop through each row
            {
                debugString += "\n|";
                for (int j = 0; j < boardWidth; j++)//loop through each column in the row
                {
                    //writes the cell and cell border
                    for (int k = 0; k < 3 - board[j, i].ToString().Length; k++)
                    {
                        debugString += (" "); // adds spacers to create a more easily readable grid
                    }
                    debugString += (board[j, i] + " |");
                }
                debugString += FillLine(boardWidth);//adds a line beteen each row to create the look of a square grid
            }
            Console.WriteLine(debugString);
        }

        //draws a grid that updates to show the knight moving around the board completing its tour, this is an async task so that task.delay can be used to have the knight move over time instead of instantly
        public async Task DrawTour()
        {
            int counter;
            for (counter = 0; counter < boardHeight * boardWidth; counter++)//repeats for every move the knight makes
            {
                string debugString = "\nTour";
                int startPosLeft = Console.CursorLeft; //remembers the start position of the cursor so that the board can be written over when updated
                int startPosTop = Console.CursorTop;

                debugString += FillLine(boardWidth); //Creates the first line

                for (int i = 0; i < boardHeight; i++) //Loops through every row
                {
                    debugString += "\n|";
                    for (int j = 0; j < boardWidth; j++) //loops throw every column in the current row
                    {
                        if (board[j, i] < counter)//if this grid square has already been shown, insert a cell with 2 solid square characters
                        {
                            debugString += (" \u25A0\u25A0 |");
                        }
                        else if (board[j, i] == counter)//if this is the current square being shown, write a grid square with the current number
                        {
                            for (int k = 0; k < 3 - board[j, i].ToString().Length; k++)
                            {
                                debugString += (" ");
                            }
                            debugString += (counter + " |");
                        }
                        else//if this square is yet to be shown, write a blank cell
                        {
                            debugString += ("    |");
                        }
                    }

                    debugString += FillLine(boardWidth);//add a line between each row to make the board more easily readable
                }
                debugString += "\n";
                Console.Write(debugString);//write the board

                Console.SetCursorPosition(startPosLeft, startPosTop);//reset the cursor position for the next write

                debugString = "";
                await Task.Delay(500);//a delay to make the board show in a more readable way
            }
            await Task.Run(DrawTour);
        }

        //used to draw a line when drawing the board, it takes the width that it will have to be
        string FillLine(int length)
        {
            string output = "";
            output += "\n+";
            for (int i = 0; i < length; i++)
            {
                output += "----+";
            }
            return output;
        }
    }
}
