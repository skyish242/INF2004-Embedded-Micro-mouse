#include "floodfill.h"

#define IS_IN_BOUNDS(x, y) ((x) >= 0 && (x) < (HEIGHT) && (y) >= 0 && (y) < (WIDTH))
bool NOT_MAP_COMPLETE = true;

bool neighbour_value_visitation(struct wall_maze* maze, struct dist_maze* distances, struct coor current, struct coor nxt_coor, int direction){
    if( IS_IN_BOUNDS(nxt_coor.x, nxt_coor.y) ){
        if(!maze->cells[current.x][current.y].walls[direction]){ // if cell has no walls 
            if(distances->distance[nxt_coor.x][nxt_coor.y] < distances->distance[current.x][current.y]){ // if next cell val is lesser than current cell value
                if(maze->cells[nxt_coor.x][nxt_coor.y].visited != 1) // Not visited
                    return true;
            }
        }
    }
    return false;
}

bool neighbour_value_distance(struct wall_maze* maze, struct dist_maze* distances, struct coor current, struct coor nxt_coor, int direction){
    if( IS_IN_BOUNDS(nxt_coor.x, nxt_coor.y) ){
        if(!maze->cells[current.x][current.y].walls[direction]){ // if cell has no walls 
            if(distances->distance[nxt_coor.x][nxt_coor.y] < distances->distance[current.x][current.y]){ // if next cell val is lesser than current cell value
                    return true;
            }
        }
    }
    return false;
}

//NORTH: n, w, e, s 
//EAST: e, n, s, w
//SOUTH: s, e, w, n
//WEST: w, s, n, e
// Define a struct to represent direction and coordinate changes
typedef struct {
    int dx; // Change in x-coordinate
    int dy; // Change in y-coordinate
} Direction;

// Define constants for directions
const Direction DIRECTIONS[] = {
    { -1, 0 },  // NORTH
    { 0, 1 },   // EAST
    { 0, -1 },  // WEST
    { 1, 0 }    // SOUTH
};

// void check_neighbour(struct wall_maze* maze, struct dist_maze* distances, struct coor current, int top, int left, int right, int bottom){
    
    // if ( IS_IN_BOUNDS(current.x, current.y) ) {
    //     // Set motor to roll back one cell length without turning 

    // }else{ // Out of bound
    //     return;
    // }

    // // if both side have walls
    // if(maze->cells[current.x][current.y].walls[left] && maze->cells[current.x][current.y].walls[right]){
    //     //roll back
    //     Direction dir = DIRECTIONS[bottom];

    //     // Update coordinates based on the direction changes
    //     current.x += dir.dx;
    //     current.y += dir.dy;
    // }else{
    //     Direction dir_left =  DIRECTIONS[left];
    //     struct coor next_coor_l = {current.x + dir_left.dx, current.y + dir_left.dy};
    //     Direction dir_right =  DIRECTIONS[right];
    //     struct coor next_coor_r = {current.x + dir_right.dx, current.y + dir_right.dy};
    //     Direction dir_btm =  DIRECTIONS[bottom];
    //     struct coor next_coor_b = {current.x + dir_btm.dx, current.y + dir_btm.dy};
        
        
    //     // Check any neighbour cell is unvisted
    
    //     if(neighbour_value_visitation(&maze,&distances, current, next_coor_l, top)){ // Check current left
    //         return;
    //     }
            
    //     if(neighbour_value_visitation(&maze,&distances, current, next_coor_r, top)){ // Check current right
    //         return;
    //     }    
    //     if(neighbour_value_visitation(&maze,&distances, current, next_coor_b, top)){// Check current bottom
    //         // Car turn right
    //         return;
    //     }

    //     // Check which neighbour cell has smaller distance (prioritise left)
    //     if(neighbour_value_distance(&maze,&distances, current, next_coor_l, top)){// Check current left
    //         // Car turn right 
    //         current = next_coor_l;
    //         check_neighbour(&maze,&distances,current, right, top, bottom, left);
    //         return;
    //     }
    //     if(neighbour_value_distance(&maze,&distances, current, next_coor_r, top)){// Check current right
    //         // Car turn left
    //         current = next_coor_r;
    //         check_neighbour(&maze,&distances,current,left, bottom, top, right);
    //         return;
    //     }
    //     if(neighbour_value_distance(&maze,&distances, current, next_coor_b, top)){// Check current bottom
    //         // Set to roll back
    //         current = next_coor_b;
    //         check_neighbour(&maze,&distances,current,top, left, right, bottom);
    //         return;
    //     }

    //     //Else all neighbour cell is visited and all neihhbour cell has higher value
    //     // Mapping completed
    //     NOT_MAP_COMPLETE = false;
    // }

    //Check from 

    // // Check if current left and right wall have walls
    // if(maze->cells[current.x][current.y].walls[left] && maze->cells[current.x][current.y].walls[right]){
    //     //roll back
    //     Direction dir = DIRECTIONS[bottom];

    //     // Update coordinates based on the direction changes
    //     current.x += dir.dx;
    //     current.y += dir.dy;
    // }else{
    //     //Check current left and right cell
    //     Direction dir =  DIRECTIONS[left];
    //     struct coor next_coor = {current.x + dir.dx, current.y + dir.dy};

    //     if (IS_IN_BOUNDS(next_coor.x, next_coor.y, WIDTH, HEIGHT)) { // Check left cell
    //         if (neighbour_value(&maze, &distances, current, next_coor, left)) { // if side cell smaller than current 
    //             if(maze)
    //             // If not visited
    //                 // return 
    //             // If visited 
    //                 // Motor turn right
    //                 next_coor.y -= 1;
    //                 return;
    //         }
    //     }
   
    // }

// }

// void check_neighbour(struct wall_maze* maze, struct dist_maze* distances, struct coor current, int currentDirection){

//     if (current.x >= 0 || current.x < HEIGHT || current.y >= 0 || current.y < WIDTH) {
//         // Set motor to roll back one cell length without turning 

//     }else{ // Out of bound
//         return;
//     }
    
//     struct coor next_coor = current;
//     bool result;
//     //Get current bearing
//     /* PRIORITIZE LEFT CELL*/
//     switch(currentDirection){
//         case NORTH:

//             if(maze->cells[current.x][current.y].walls[WEST] && maze->cells[current.x][current.y].walls[EAST]){
//                 // roll back
//                 current.x += 1;
//                 check_neighbour(&maze, &distances, current, currentDirection);
//             }else{
//                 if (IS_IN_BOUNDS(current.x, current.y-1, WIDTH, HEIGHT)) { // Check left cell
//                     if (neighbour_value(&maze, &distances, current, (struct coor){current.x, current.y - 1}, WEST)) { // if side cell smaller than current 
//                         // If not visited
//                             // return 
//                         // If visited 
//                             // Motor turn right
//                             next_coor.y -= 1;
//                             return;
//                     }
//                 }

//                 if (IS_IN_BOUNDS(current.x, current.y+1, WIDTH, HEIGHT)) { // Check right cell
//                     if (neighbour_value(&maze, &distances, current, (struct coor){current.x, current.y + 1}, EAST)) {
//                         return;
//                     }
//                 }

//                 // Motor turn right

//                 if (IS_IN_BOUNDS(current.x - 1, current.y, WIDTH, HEIGHT)) { // Check the other left cell
//                     if (neighbour_value(&maze, &distances, current, (struct coor){current.x - 1, current.y}, NORTH)) {
//                         return;
//                     }
//                 }

//                 if (IS_IN_BOUNDS(current.x + 1, current.y, WIDTH, HEIGHT)) { // Check the other right cell
//                     if (neighbour_value(&maze, &distances, current, (struct coor){current.x + 1, current.y}, SOUTH)) {
//                         return;
//                     }
//                 }  
//             }

//             break;

//         case EAST:
//             if(maze->cells[current.x][current.y].walls[NORTH] && maze->cells[current.x][current.y].walls[SOUTH]){
//                 // roll back
//                 current.y -= 1;
//                 check_neighbour(&maze, &distances, current, currentDirection);
//             }else{
//                 if (IS_IN_BOUNDS(current.x- 1, current.y, WIDTH, HEIGHT)) { // Check left cell
//                     if (neighbour_value(&maze, &distances, current, (struct coor){current.x-1, current.y}, NORTH)) {
//                         return;
//                     }
//                 }

//                 if (IS_IN_BOUNDS(current.x + 1, current.y, WIDTH, HEIGHT)) { // Check right cell
//                     if (neighbour_value(&maze, &distances, current, (struct coor){current.x+1, current.y}, SOUTH)) {
//                         return;
//                     }
//                 }

//                 // Motor turn right

//                 if (IS_IN_BOUNDS(current.x, current.y + 1, WIDTH, HEIGHT)) { // Check the other left cell
//                     if (neighbour_value(&maze, &distances, current, (struct coor){current.x, current.y+1}, EAST)) {
//                         return;
//                     }
//                 }

//                 if (IS_IN_BOUNDS(current.x, current.y-1, WIDTH, HEIGHT)) { // Check the other right cell
//                     if (neighbour_value(&maze, &distances, current, (struct coor){current.x, current.y-1}, WEST)) {
//                         return;
//                     }
//                 }  
//             }

//             break;
//         case SOUTH: 

//             if(maze->cells[current.x][current.y].walls[EAST] && maze->cells[current.x][current.y].walls[WEST]){
//                 // roll back
//                 current.x -= 1;
//                 check_neighbour(&maze, &distances, current, currentDirection);
//             }else{
//                 if (IS_IN_BOUNDS(current.x, current.y+1, WIDTH, HEIGHT)) { // Check left cell
//                     if (neighbour_value(&maze, &distances, current, (struct coor){current.x, current.y + 1}, EAST)) {
//                         return;
//                     }
//                 }

//                 if (IS_IN_BOUNDS(current.x, current.y-1, WIDTH, HEIGHT)) { // Check right cell
//                     if (neighbour_value(&maze, &distances, current, (struct coor){current.x, current.y - 1}, WEST)) {
//                         return;
//                     }
//                 }

//                 // Motor turn right

//                 if (IS_IN_BOUNDS(current.x + 1, current.y, WIDTH, HEIGHT)) { // Check the other left cell
//                     if (neighbour_value(&maze, &distances, current, (struct coor){current.x + 1, current.y}, SOUTH)) {
//                         return;
//                     }
//                 }

//                 if (IS_IN_BOUNDS(current.x - 1, current.y, WIDTH, HEIGHT)) { // Check the other right cell
//                     if (neighbour_value(&maze, &distances, current, (struct coor){current.x - 1, current.y}, NORTH)) {
//                         return;
//                     }
//                 }  
//                 //Mapping finish
//             }

//             break;
//         case WEST:

//             if(maze->cells[current.x][current.y].walls[SOUTH] && maze->cells[current.x][current.y].walls[NORTH]){
//                 // roll back
//                 current.y += 1;
//                 check_neighbour(&maze, &distances, current, currentDirection);
//             }else{
//                 if (IS_IN_BOUNDS(current.x+ 1, current.y, WIDTH, HEIGHT)) { // Check left cell
//                     if (neighbour_value(&maze, &distances, current, (struct coor){current.x+1, current.y}, NORTH)) {
//                         return;
//                     }
//                 }

//                 if (IS_IN_BOUNDS(current.x - 1, current.y, WIDTH, HEIGHT)) { // Check right cell
//                     if (neighbour_value(&maze, &distances, current, (struct coor){current.x-1, current.y}, SOUTH)) {
//                         return;
//                     }
//                 }

//                 // Motor turn right

//                 if (IS_IN_BOUNDS(current.x, current.y - 1, WIDTH, HEIGHT)) { // Check the other left cell
//                     if (neighbour_value(&maze, &distances, current, (struct coor){current.x, current.y+1}, WEST)) {
//                         return;
//                     }
//                 }

//                 if (IS_IN_BOUNDS(current.x, current.y + 1, WIDTH, HEIGHT)) { // Check the other right cell
//                     if (neighbour_value(&maze, &distances, current, (struct coor){current.x, current.y-1}, EAST)) {
//                         return;
//                     }
//                 }  
//             }

//             break;
//         default:
//             break;
//     }
// }

void advance_one_cell(struct wall_maze* maze, struct dist_maze* distances, struct coor current, int currentDirection){

    int cur_encoder_value = 0;
    struct coor next_coord = current;
    // bool left_wall = 0;
    // bool right_wall = 0;

    // Moving forward
    // Call motor to move forward one cell and get encoder value back continously
    moveOneCell();
    printf("Leftwall: %d\n", left_wall);
    printf("Rightwall: %d\n", right_wall);

    // Check the current direction
    // Set the wall of the current cell the robot is at 
    // switch(currentDirection){
    //     case NORTH:
    //         // Set current wall base on the truth value of the wall
    //         maze->cells[current.x][current.y].walls[EAST] = right_wall;
    //         maze->cells[current.x][current.y].walls[WEST] = left_wall;
            
    //         // Set adjacent cells wall
    //         if (current.y < WIDTH-1) {
    //             // Set the adjacent right cell
    //             maze->cells[current.x][current.y+1].walls[WEST] = right_wall;
    //         }
    //         if(current.y > 0){
    //             // Set the adjacent left cell
    //             maze->cells[current.x][current.y-1].walls[EAST] = left_wall;                
    //         }

    //         // Set cell's distance
    //         if(current.x < HEIGHT-1){
    //             int prev_dist = distances->distance[current.x+1][current.y];
    //             // Set current distance distance from the previous cell
    //             distances->distance[current.x][current.y] = prev_dist + 1;
    //         }
    //         break;
    //     case EAST:
    //         // Set current wall base on the truth value of the wall
    //         maze->cells[current.x][current.y].walls[SOUTH] = right_wall;
    //         maze->cells[current.x][current.y].walls[NORTH] = left_wall;
            
    //         // Set adjacent cells wall
    //         if (current.x < HEIGHT-1 ) {
    //             // Set the adjacent right cell
    //             maze->cells[current.x+1][current.y].walls[NORTH] = right_wall;                             
    //         }
    //         if(current.x > 0){
    //             // Set the adjacent left cell
    //             maze->cells[current.x-1][current.y].walls[SOUTH] = left_wall;               
    //         }
            
    //         // Set cell's distance
    //         if(current.y > 0){
    //             int prev_dist = distances->distance[current.x][current.y-1];
    //             // Set current distance distance from the previous cell
    //             distances->distance[current.x][current.y] = prev_dist + 1;
    //         }
    //         break;
    //     case SOUTH:
    //         // Set current wall base on the truth value of the wall
    //         maze->cells[current.x][current.y].walls[WEST] = right_wall;
    //         maze->cells[current.x][current.y].walls[EAST] = left_wall;
            
    //         // Set adjacent cells wall
    //         if(current.y > 0){
    //             // Set the adjacent right cell
    //             maze->cells[current.x][current.y-1].walls[EAST] = right_wall;           
    //         }
    //         if (current.y < WIDTH-1) {
    //             // Set the adjacent left cell
    //             maze->cells[current.x][current.y+1].walls[WEST] = left_wall;         
    //         }

    //         // Set cell's distance
    //         if(current.x > 0){
    //             int prev_dist = distances->distance[current.x-1][current.y];
    //             // Set current distance distance from the previous cell
    //             distances->distance[current.x][current.y] = prev_dist + 1;
    //         }
    //         break;
    //     case WEST:
    //         // Set current wall 
    //         maze->cells[current.x][current.y].walls[NORTH] = right_wall;
    //         maze->cells[current.x][current.y].walls[SOUTH] = left_wall;
            
    //         // Set adjacent cells wall         
    //         if(current.x > 0){
    //             // Set the adjacent right cell
    //             maze->cells[current.x-1][current.y].walls[SOUTH] = right_wall;
    //         }   
    //         if (current.x < HEIGHT-1) {
    //             // Set the adjacent left cell
    //             maze->cells[current.x+1][current.y].walls[NORTH] = left_wall;                
    //         }
            
    //         // Set cell's distance
    //         if(current.y < WIDTH-1){
    //             int prev_dist = distances->distance[current.x][current.y+1];
    //             // Set current distance distance from the previous cell
    //             distances->distance[current.x][current.y] = prev_dist + 1;
    //         }
    //         break;
    //     default:
    //         break;
    // }
    
    // // Set cell as visited
    // maze->cells[current.x][current.y].visited = VISITED;

}
void floodfill_mapping(struct wall_maze* maze, struct dist_maze* distances, struct coor current, int currentDirection){
    //If start outside of maze, then advance in first
    advance_one_cell(&maze, &distances, current, currentDirection);
    // Set starting distance
    // distances->distance[current.x][current.y] = 0;
    
    // struct coor next_coor = current;
    // while(NOT_MAP_COMPLETE){
    //     // Check if there is opening (no wall) around current cell
    //     switch(currentDirection){
    //         case NORTH:
    //             if (!maze->cells[current.x][current.y].walls[EAST]){ // No walls on the right 
    //                 if(current.y+1 < WIDTH){ // If right is not out of bound
    //                     if(!maze->cells[current.x][current.y+1].visited){ // If right is not visited
    //                         // Motor turn right
    //                         // While turning check if left and right IR have wall

    //                         // Set next coord
    //                         next_coor.y += 1;
    //                         currentDirection = EAST;
    //                         advance_one_cell(&maze, &distances, next_coor, currentDirection);
    //                         continue;
    //                     }
    //                 }
    //             }
    //             if (!maze->cells[current.x][current.y].walls[WEST]){ // No walls on the left
    //                 if(current.y-1 >= 0){ // Check if left is not out of bound
    //                     if(!maze->cells[current.x][current.y-1].visited){ // if left is not visited 
    //                         // Motor turn left
    //                         // While turning check if left and right IR have wall
    //                             // if wall, set wall
    //                         // Set next coord
    //                         next_coor.y -= 1;
    //                         currentDirection = WEST;
    //                         advance_one_cell(&maze, &distances, next_coor, currentDirection);
    //                         continue;
    //                     }
    //                 }
    //             }else{ // Both side have walls
    //                 if(current.x-1 >= 0){ // Check if forward is not out of bounds
    //                     if(!maze->cells[current.x-1][current.y].visited){ // Check if front is not visited 
    //                         // Turn right check if there is front wall
    //                         // If no walls
    //                             // Turn back left 
    //                             next_coor.x -=1;
    //                             advance_one_cell(&maze, &distances, next_coor, currentDirection);
    //                             continue;
    //                         // If walls and not visited
    //                         // Need to start backtracking
    //                             next_coor.x += 1;
    //                             check_neighbour(&maze, &distances,next_coor,NORTH,WEST,EAST,SOUTH);
    //                             continue;
    //                     }else{ // front is visited
    //                         next_coor.x +=1;
    //                         check_neighbour(&maze, &distances,next_coor,NORTH,WEST,EAST,SOUTH);
    //                         continue;
    //                     }
    //                 }else{ // Forward is out of bound, move back
    //                     next_coor.x += 1;
    //                     check_neighbour(&maze, &distances,next_coor,NORTH,WEST,EAST,SOUTH);
    //                     continue;
    //                 }
    //             }
    //             break;

    //         case EAST:
    //             if (!maze->cells[current.x][current.y].walls[SOUTH]){ // No walls on the right 
    //                 if(current.x+1 < HEIGHT){ // If right is not out of bound
    //                     if(!maze->cells[current.x+1][current.y].visited){ // If right is not visited
    //                         // Motor turn right
    //                         // While turning check if left and right IR have wall

    //                         // Set next coord
    //                         next_coor.x += 1;
    //                         currentDirection = SOUTH;
    //                         advance_one_cell(&maze, &distances, next_coor, currentDirection);
    //                         continue;
    //                     }
    //                 }
    //             }
    //             if (!maze->cells[current.x][current.y].walls[NORTH]){ // No walls on the left
    //                 if(current.x-1 >= 0){ // Check if left is not out of bound
    //                     if(!maze->cells[current.x-1][current.y].visited){ // if left is not visited 
    //                         // Motor turn left
    //                         // While turning check if left and right IR have wall
                            
    //                         // Set next coord
    //                         next_coor.x -= 1;
    //                         currentDirection = NORTH;
    //                         advance_one_cell(&maze, &distances, next_coor, currentDirection);
    //                         continue;
    //                     }
    //                 }
    //             }else{ // Both side have walls
    //                 if(current.y+1 < WIDTH){ // Check if forward is not out of bounds
    //                      if(!maze->cells[current.x][current.y+1].visited){ // Check if front is not visited 
    //                         // Turn right check if there is front wall
    //                         // If no walls
    //                             // Turn back left 
    //                             next_coor.y +=1;
    //                             advance_one_cell(&maze, &distances, next_coor, currentDirection);
    //                             continue;;
    //                         // If walls and not visited
    //                             next_coor.y -= 1;
    //                             check_neighbour(&maze, &distances, next_coor, EAST, NORTH, SOUTH, WEST);
    //                             continue;
    //                      }else{ // Visited already
    //                         next_coor.y -= 1;
    //                         check_neighbour(&maze, &distances, next_coor, EAST, NORTH, SOUTH, WEST);
    //                         continue;
    //                      }
    //                 }else{ // Forward is out of bound, move back
    //                     next_coor.x += 1;
    //                     check_neighbour(&maze, &distances,next_coor,EAST, NORTH, SOUTH, WEST);
    //                     continue;
    //                 }
    //             }
    //             break;

    //         case SOUTH:
    //             if (!maze->cells[current.x][current.y].walls[WEST]){ // No walls on the right 
    //                 if(current.y-1 >= 0){ // If right is not out of bound
    //                     if(!maze->cells[current.x][current.y-1].visited){ // If right is not visited
    //                         // Motor turn right
    //                         // While turning check if left and right IR have wall

    //                         // Set next coord
    //                         next_coor.y -= 1;
    //                         currentDirection = WEST;
    //                         advance_one_cell(&maze, &distances, next_coor, currentDirection);
    //                         continue;
    //                     }
    //                 }
    //             }
    //             if (!maze->cells[current.x][current.y].walls[EAST]){ // No walls on the left
    //                 if(current.y+1 < HEIGHT){ // Check if left is not out of bound
    //                     if(!maze->cells[current.x][current.y+1].visited){ // if left is not visited 
    //                         // Motor turn left
    //                         // While turning check if left and right IR have wall
                            
    //                         // Set next coord
    //                         next_coor.y += 1;
    //                         currentDirection = EAST;
    //                         advance_one_cell(&maze, &distances, next_coor, currentDirection);
    //                         continue;
    //                     }
    //                 }
    //             }else{ // Both side have walls
    //                 if(current.x+1 < HEIGHT){ // Check if forward is not out of bounds
    //                      if(!maze->cells[current.x+1][current.y].visited){ // Check if front is not visited 
    //                         // Turn right check if there is front wall
    //                         // If no walls
    //                             // Turn back left 
    //                             next_coor.x +=1;
    //                             advance_one_cell(&maze, &distances, next_coor, currentDirection);
    //                             continue;;
    //                         // If walls and not visited
    //                             next_coor.x -= 1;
    //                             check_neighbour(&maze, &distances, next_coor, SOUTH, EAST, WEST, NORTH);
    //                             continue;
    //                      }else{ //Visited already
    //                         next_coor.x -= 1;
    //                         check_neighbour(&maze, &distances, next_coor,SOUTH, EAST, WEST, NORTH);
    //                         continue;
    //                      }
    //                 }else{ // Forward is out of bound, move back
    //                     next_coor.x += 1;
    //                     check_neighbour(&maze, &distances,next_coor,SOUTH, EAST, WEST, NORTH);
    //                     continue;
    //                 }
    //             }
    //             break;

    //         case WEST:  
    //             if (!maze->cells[current.x][current.y].walls[NORTH]){ // No walls on the right 
    //                 if(current.x-1 >= 0){ // If right is not out of bound
    //                     if(!maze->cells[current.x-1][current.y].visited){ // If right is not visited
    //                         // Motor turn right
    //                         // While turning check if left and right IR have wall

    //                         // Set next coord
    //                         next_coor.x -= 1;
    //                         currentDirection = NORTH;
    //                         advance_one_cell(&maze, &distances, next_coor, currentDirection);
    //                         continue;
    //                     }
    //                 }
    //             } 
    //             if (!maze->cells[current.x][current.y].walls[SOUTH]){ // No walls on the left
    //                 if(current.x+1 < HEIGHT){ // Check if left is not out of bound
    //                     if(!maze->cells[current.x+1][current.y].visited){ // if left is not visited 
    //                         // Motor turn left
    //                         // While turning check if left and right IR have wall
                            
    //                         // Set next coord
    //                         next_coor.x += 1;
    //                         currentDirection = SOUTH;
    //                         advance_one_cell(&maze, &distances, next_coor, currentDirection);
    //                         continue;
    //                     }
    //                 }
    //             }else{ // Both side have walls
    //                 if(current.y-1 >= 0){ // Check if forward is not out of bounds
    //                      if(!maze->cells[current.x][current.y-1].visited){ // Check if front is not visited 
    //                         // Turn right check if there is front wall
    //                         // If no walls
    //                             // Turn back left 
    //                             next_coor.y -=1;
    //                             advance_one_cell(&maze, &distances, next_coor, currentDirection);
    //                             continue;;
    //                         // If walls and not visited
    //                             next_coor.y += 1;
    //                             check_neighbour(&maze, &distances,  next_coor, WEST, SOUTH, NORTH, EAST);
    //                             continue;
    //                      }else{ //Visited already
    //                         next_coor.y += 1;
    //                         check_neighbour(&maze, &distances, next_coor, WEST, SOUTH, NORTH, EAST);
    //                         continue;
    //                      }
    //                 }else{ // Forward is out of bound, move back
    //                     next_coor.x += 1;
    //                     check_neighbour(&maze, &distances,next_coor,WEST, SOUTH, NORTH, EAST);
    //                     continue;
    //                 }
    //             }
    //             break;
    //         default:
    //             break;
    //     }
    // }
}

void mapping_start(){

    // Initialize the original maze
    struct wall_maze maze;
    for (int i = 0; i < HEIGHT; i++) {
        for (int j = 0; j < WIDTH; j++) {
            for (int k = 0; k < 4; k++) {
                maze.cells[i][j].walls[k] = 0; // Set all walls to false initially
            }
            maze.cells[i][j].visited = false; // Set visited to false initially
        }
    }

    struct dist_maze distances;
    for (int i = 0; i < HEIGHT; i++) {
        for (int j = 0; j < WIDTH; j++) {
            distances.distance[i][j] = UNKNOWN; // Set distances to -1 initially 
        }
    }

    //Start of the MAZE
    struct coor start;
    start.x = START_X;
    start.y = START_Y;

    // //Initialize the queue
    // initializeQueue(&flood_queue);

    // // Enqueue the first cell
    // enqueue(&flood_queue, start);


    floodfill_mapping(&maze, &distances, start, NORTH);
}

// switch(currentDirection){
//     case NORTH:
//         break;
//     case EAST:
//         break;
//     case SOUTH:
//         break;
//     case WEST:
//         break;
    // default:
    //     break;
// }