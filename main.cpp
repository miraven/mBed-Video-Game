// Project includes
#include "globals.h"
#include "hardware.h"
#include "map.h"
#include "graphics.h"
#include "speech.h"

// Functions in this file
int get_action (GameInputs inputs);
int update_game (int action);
void draw_game (int init);
void init_main_map ();
int main ();

//How many trees 
int count = 0; 

/**
 * The main game state. Must include Player locations and previous locations for
 * drawing to work properly. Other items can be added as needed.
 */
struct {
    int x,y;    // Current locations
    int px, py; // Previous locations
    int has_key;
} Player;

/**
 * Given the game inputs, determine what kind of update needs to happen.
 * Possbile return values are defined below.
 */
#define NO_ACTION 0
#define ACTION_BUTTON 1
#define OMNIPOTENT_BUTTON 2
#define GO_LEFT 3
#define GO_RIGHT 4
#define GO_UP 5
#define GO_DOWN 6
int get_action(GameInputs inputs)
{
    if (!inputs.b1) 
        return OMNIPOTENT_BUTTON; 
    else if (!inputs.b2) 
        return ACTION_BUTTON;
    else if (inputs.ax > 0 && inputs.ax > inputs.ay && inputs.ax > inputs.az) 
        return GO_RIGHT;
    else if (inputs.ax < 0 && inputs.ax < inputs.ay && inputs.ax < inputs.az)
        return GO_LEFT; 
    else if (inputs.ay > 0 && inputs.ay > inputs.ax && inputs.ay > inputs.az)
        return GO_UP;
    else if (inputs.ay < 0 && inputs.ay < inputs.ax && inputs.ay < inputs.az) 
        return GO_DOWN;
    //else if (inputs.az > inputs.ax && inputs.az > inputs.ay)
    else
        return NO_ACTION;
            
}

/**
 * Update the game state based on the user action. For example, if the user
 * requests GO_UP, then this function should determine if that is possible by
 * consulting the map, and update the Player position accordingly.
 * 
 * Return values are defined below. FULL_DRAW indicates that for this frame,
 * draw_game should not optimize drawing and should draw every tile, even if
 * the player has not moved.
 */
#define NO_RESULT 0
#define GAME_OVER 1
#define FULL_DRAW 2
int update_game(int action)
{
    // Save player previous location before updating
    Player.px = Player.x;
    Player.py = Player.y;
    
    // Do different things based on the each action.
    // You can define functions like "go_up()" that get called for each case.
    switch(action)
    {
        case GO_UP:
            // update 
            MapItem* checkUp = get_north(Player.x, Player.y); 
            if (checkUp -> walkable)
                Player.y = Player.y - 1; 
                return FULL_DRAW;
        case GO_LEFT:  
            MapItem* checkLeft = get_west(Player.x, Player.y); 
            if (checkLeft -> walkable)
                Player.x = Player.x - 1; 
                return FULL_DRAW;        
        case GO_DOWN:   
            MapItem* checkDown = get_south(Player.x, Player.y); 
            if (checkDown -> walkable) 
                Player.y = Player.y + 1; 
                return FULL_DRAW;
        case GO_RIGHT:  
            MapItem* checkRight = get_east(Player.x, Player.y); 
            if (checkRight -> walkable) 
                Player.x = Player.x + 1; 
                return FULL_DRAW;
        case ACTION_BUTTON:
            MapItem* north = get_north (Player.x, Player.y);
            MapItem* south = get_south (Player.x, Player.y);
            MapItem* east = get_east (Player.x, Player.y);
            MapItem* west = get_west (Player.x, Player.y);
            if (north -> type == 5){
                north -> type = NULL;
                north -> draw = draw_nothing;
                Player.has_key = 1; 
                return FULL_DRAW;
            }
            else if (south -> type == 5){
                south -> type = NULL;
                south -> draw = draw_nothing;
                Player.has_key = 1; 
                return FULL_DRAW;
            }
            else if (east -> type == 5){
                east -> type = NULL;
                east -> draw = draw_nothing;
                Player.has_key = 1;
                return FULL_DRAW; 
            }
            else if (west -> type == 5){
                west -> type = NULL;
                west -> draw = draw_nothing;
                Player.has_key = 1; 
                return FULL_DRAW;
            }
            
            else if (north -> type == 3 ||south -> type == 3 || east -> type == 3 || west -> type == 3 ) {
                if (Player.has_key == 1) {
                    // Go check out abandoned house
                    speech("Ok thanks.", "We still need.."); 
                    speech("to leave this", "this island."); 
                    speech("You're an", "engineer..");
                    speech("..right?", "Build a boat.");
                    speech("There's a house", "at the top..");
                    speech(".. right of the", "island.");
                    speech("There might be", "a pickaxe.");
                    return FULL_DRAW;
                }
                
                else if (Player.has_key == 2) {
                    speech("Oh good.", "You found an axe."); 
                    speech("Now just cut", "up some wood."); 
                    speech(".. maybe five", "trees?"); 
                    return FULL_DRAW;
                } 
                
                else if (Player.has_key == 3) {
                    speech("Yes!", "You saved us!"); 
                    speech("At last,", "We can leave!"); 
                    return GAME_OVER;
                }
                
                else {
                    speech("Hey, kid", "are you ok?");
                    speech("I don't know", "if you remember");
                    speech("but the plane", "crashed."); 
                    speech("People are hurt.", "We need a med kit");
                    speech("Find it for me.", "It's around here");
                    return FULL_DRAW; 
                }
            }
            
            
            else if (north -> type == 4) {
                if (Player.has_key == 1) {
                    // Go check out abandoned house
                    north -> type = NULL;
                    north -> walkable = true;
                    north -> draw = draw_nothing;
                }
                return FULL_DRAW;
            }
            
            else if (north -> type == 6 ||south -> type == 6 || east -> type == 6 || west -> type == 6) {
                if (Player.has_key == 1){
                    //Game over
                    //return GAME_OVER;
                    Player.has_key = 2;
                    north -> draw = draw_nothing;
                    north -> type = NULL; 
                    return FULL_DRAW;
                }
                else
                    return FULL_DRAW;
            }
            // Cut down trees
            else if ((north -> type == 1) && (Player.has_key == 2)) {
                north -> type = NULL;
                north -> draw = draw_nothing;
                if (count == 5) {
                    Player.has_key = 3; 
                    return FULL_DRAW;
                }
                else {
                    count++;
                    return FULL_DRAW;
                }
            }
            else if ((south -> type == 1) && (Player.has_key == 2)) {
                south -> type = NULL;
                south -> draw = draw_nothing;
                if (count == 5) {
                    Player.has_key = 3; 
                    return FULL_DRAW;
                }
                else {
                    count++;
                    return FULL_DRAW;
                } 
            }
            else if ((east -> type == 1) && (Player.has_key == 2)) {
                east -> type = NULL;
                east -> draw = draw_nothing;
                if (count == 5) {
                    Player.has_key = 3; 
                    return FULL_DRAW;
                }
                else {
                    count++;
                    return FULL_DRAW; 
                }
            }
            else if ((west -> type == 1) && (Player.has_key == 2)) {
                west -> type = NULL;
                west -> draw = draw_nothing;
                if (count == 5) {
                    Player.has_key = 3; 
                    return FULL_DRAW;
                }
                else {
                    count++;
                    return FULL_DRAW;
                }
            }

        case OMNIPOTENT_BUTTON:
            for (int i = 0; i < 80; i++) {
                for (int j = 0 ; j <80; j++) {
                    MapItem* here = get_here(i,j);
                    if (here -> type == 0 || here -> type == 2 || here -> type == 3 || here -> type == 4 || here -> type == 5 || here -> type == 6 ) 
                        here -> walkable = true;
                }   
            }
            return FULL_DRAW;
        default:        break;
    }
    return NO_RESULT;
}

/**
 * Entry point for frame drawing. This should be called once per iteration of
 * the game loop. This draws all tiles on the screen, followed by the status 
 * bars. Unless init is nonzero, this function will optimize drawing by only 
 * drawing tiles that have changed from the previous frame.
 */
void draw_game(int init)
{
    // Draw game border first
    if(init) draw_border();
    
    // Iterate over all visible map tiles
    for (int i = -5; i <= 5; i++) // Iterate over columns of tiles
    {
        for (int j = -4; j <= 4; j++) // Iterate over one column of tiles
        {
            // Here, we have a given (i,j)
            
            // Compute the current map (x,y) of this tile
            int x = i + Player.x;
            int y = j + Player.y;
            
            // Compute the previous map (px, py) of this tile
            int px = i + Player.px;
            int py = j + Player.py;
                        
            // Compute u,v coordinates for drawing
            int u = (i+5)*11 + 3;
            int v = (j+4)*11 + 15;
            
            // Figure out what to draw
            DrawFunc draw = NULL;
            if (init && i == 0 && j == 0) // Only draw the player on init
            {
                draw_player(u, v, Player.has_key);
                continue;
            }
            else if (x >= 0 && y >= 0 && x < map_width() && y < map_height()) // Current (i,j) in the map
            {
                MapItem* curr_item = get_here(x, y);
                MapItem* prev_item = get_here(px, py);
                if (init || curr_item != prev_item) // Only draw if they're different
                {
                    if (curr_item) // There's something here! Draw it
                    {
                        draw = curr_item->draw;
                    }
                    else // There used to be something, but now there isn't
                    {
                        draw = draw_nothing;
                    }
                }
            }
            else if (init) // If doing a full draw, but we're out of bounds, draw the walls.
            {
                draw = draw_wall;
            }

            // Actually draw the tile
            if (draw) draw(u, v);
        }
    }

    // Draw status bars    
    draw_upper_status();
    draw_lower_status();
}


/**
 * Initialize the main world map. Add walls around the edges, interior chambers,
 * and plants in the background so you can see motion.
 */
void init_main_map()
{
    /* "Random" plants
    //Map* map = set_active_map(0);
    for(int i = map_width() + 3; i < map_area(); i += 19)
    {
        // Plant trees around the wreckage
        if ((i % map_width() < 30 || i % map_width() > 40)){
            if ((i / map_width() < 30 || i / map_width() > 40)){
                MapItem* here = get_here (i % map_width(), i / map_width());
                if (!(here -> type == 0 || here -> type == 2 || here -> type == 3))
                    add_plant(i % map_width(), i / map_width());
            }
        }
    }
    pc.printf("plants\r\n");
    */
    
    
    // Add survivor at crash
    add_survivor(30, 28);
     
    pc.printf("Adding walls!\r\n");
    add_wall(0,              0,              HORIZONTAL, map_width());
    add_wall(0,              map_height()-1, HORIZONTAL, map_width());
    add_wall(0,              0,              VERTICAL,   map_height());
    add_wall(map_width()-1,  0,              VERTICAL,   map_height());
    pc.printf("Walls done!\r\n");
    
    // Draw plane crash
    for (int s = 30; s < 37; s++) {
        for (int r = 30; r < 37; r++) {
            add_debri(s,r);
        }
    }
    
    // Draw abandoned house 
    for (int g = 0; g < 10; g++) {
        add_wall(65, 0, VERTICAL, 10); 
        add_wall(72, 0, VERTICAL, 10); 
        add_wall(65, 10, HORIZONTAL,4);
        add_door(68,10); 
        add_wall(69,10,HORIZONTAL,4); 
    }
    
    
    // Draw debri crumbs to plane crash 
    for (int q = 0; q < 22; q+=2) {
        MapItem* here = get_here (7+q, 8+q); 
        if (here) 
            add_debri (7+q+1, 8+q); // add next to tree 
        else 
            add_debri (7+q, 8+q);
    }
    
/*
    for (int z = 0; z < 200; z++) {
        int ix = rand() % 80; 
        int jy = rand() % 80; 
        MapItem* here = get_here(ix, jy); 
        if (!(here -> type == 0 || here -> type == 2 || here -> type == 3))
            add_plant(ix,jy);
    }
*/

// Add plants 
    for(int i = map_width() + 3; i < map_area(); i += 19)
    {
        // Plant trees around the wreckage
        if ((i % map_width() < 30 || i % map_width() > 40) && (i / map_width() < 30 || i / map_width() > 40)){
            
            MapItem* here = get_here (i % map_width(), i / map_width());
            if (!(here -> type == 0 || here -> type == 2 || here -> type == 3))
                add_plant(i % map_width(), i / map_width());
        }
    }
    add_medkit (43,35); 
    
    MapItem* herexy = get_here(68,3); 
    if (herexy -> type == NULL) 
        add_axe(69, 3); 
    else 
        add_axe(68,3);   
    
    
    pc.printf("plants\r\n");
    Map* map = set_active_map(0);

    print_map();
}

/**
 * Program entry point! This is where it all begins.
 * This function orchestrates all the parts of the game. Most of your
 * implementation should be elsewhere - this holds the game loop, and should
 * read like a road map for the rest of the code.
 */
int main()
{
    // First things first: initialize hardware
    ASSERT_P(hardware_init() == ERROR_NONE, "Hardware init failed!");

    // Initialize the maps
    maps_init();
    init_main_map();
    
    // Initialize game state
    set_active_map(0);
    Player.x = Player.y = 5;

    // Initial drawing
    draw_game(true);

    // Main game loop
    while(1)
    {
        // Timer to measure game update speed
        Timer t; t.start();
        
        // Actuall do the game update:
        uLCD.locate(1,0);
        uLCD.printf("x:%d y:%d", Player.x, Player.y);
        // 1. Read inputs
        GameInputs inputs = read_inputs();
                
        // 2. Determine action (get_action)  
        int action = get_action(inputs);
        
        // 3. Update game (update_game)
        int draw = update_game(action); 
        // 3b. Check for game over
        if (draw == 1) { // Game Over 
            uLCD.filled_rectangle(0, 0, 127, 127, WHITE); 
            uLCD.locate(4,7);
            uLCD.printf("GAME OVER!"); 
            break;
        }
        // 4. Draw frame (draw_game)
        draw_game(draw);
        
        // 5. Frame delay
        t.stop();
        int dt = t.read_ms();
        if (dt < 100) wait_ms(100 - dt);
    }
}
