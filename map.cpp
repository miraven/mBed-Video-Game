#include "map.h"

#include "globals.h"
#include "graphics.h"

/**
 * The Map structure. This holds a HashTable for all the MapItems, along with
 * values for the width and height of the Map.
 */
struct Map {
    HashTable* items; // pointer to hash table object 
    int w, h;
};

/**
 * Storage area for the maps.
 * This is a global variable, but can only be access from this file because it
 * is static.
 */
static Map map;
static int active_map;

/**
 * The first step in HashTable access for the map is turning the two-dimensional
 * key information (x, y) into a one-dimensional unsigned integer.
 * This function should uniquely map (x,y) onto the space of unsigned integers.
 */
static unsigned XY_KEY(int X, int Y) { 
    // Mulitply num_buckets * int X + int Y 
    return (80*Y) + X; 
}

/**
 * This is the hash function actually passed into createHashTable. It takes an
 * unsigned key (the output of XY_KEY) and turns it into a hash value (some
 * small non-negative integer).
 */
unsigned map_hash(unsigned key) // Tell which bucket the key is in
{
    // Hash function is mod of number of buckets or the width/height of map
    return key%80; 
}

void maps_init()
{   
    // Set width & height
    map.w = 80; 
    map.h = 80; 
    
    // Initialize hash table
    map.items = createHashTable(&map_hash, 80); 
}

Map* get_active_map()
{
    // There's only one map
    return &map;
}

// DOES NOTHING... so far.
Map* set_active_map(int m)
{
    active_map = m;
    return &map;
}

void print_map()
{
    // As you add more types, you'll need to add more items to this array.
    char lookup[] = {'W', 'P'};
    for(int y = 0; y < map_height(); y++)
    {
        for (int x = 0; x < map_width(); x++)
        {
            MapItem* item = get_here(x,y);
            if (item) pc.printf("%c", lookup[item->type]);
            else pc.printf(" ");
        }
        pc.printf("\r\n");
    }
}

int map_width()
{
    return map.w; 
}

int map_height()
{
    return map.h;
}

int map_area()
{
    int area = (map.w)*(map.h); 
    return area;
}

MapItem* get_north(int x, int y)
{
    unsigned key = XY_KEY (x, y-1); // concatenates the coordinates
    HashTable* table = get_active_map() -> items; 
    MapItem* item = (MapItem*)getItem(table, key); 
    return item;
}

MapItem* get_south(int x, int y)
{
    unsigned key = XY_KEY (x, y+1); // concatenates the coordinates
    HashTable* table = get_active_map() -> items; 
    MapItem* item = (MapItem*)getItem(table, key); 
    return item; 
}

MapItem* get_east(int x, int y)
{
    unsigned key = XY_KEY (x+1, y); // concatenates the coordinates
    HashTable* table = get_active_map() -> items; 
    MapItem* item = (MapItem*)getItem(table, key);  
    return item;  
}

MapItem* get_west(int x, int y)
{
    unsigned key = XY_KEY (x-1, y); // concatenates the coordinates
    HashTable* table = get_active_map() -> items; 
    MapItem* item = (MapItem*)getItem(table, key);  
    return item; 
}

MapItem* get_here(int x, int y)
{
    unsigned key = XY_KEY (x, y); // concatenates the coordinates
    HashTable* table = map.items; 
    MapItem* item = (MapItem*)getItem(table, key);  
    return item; 
}


void map_erase(int x, int y)
{
    unsigned key = XY_KEY (x, y); // concatenates the coordinates
    HashTable* table = get_active_map() -> items;  
    deleteItem (table, key); 
}

void add_wall(int x, int y, int dir, int len)
{
    for(int i = 0; i < len; i++)
    {
        MapItem* w1 = (MapItem*) malloc(sizeof(MapItem));
        w1->type = WALL;
        w1->draw = draw_wall;
        w1->walkable = false;
        w1->data = NULL;
        unsigned key = (dir == HORIZONTAL) ? XY_KEY(x+i, y) : XY_KEY(x, y+i);
        void* val = insertItem(get_active_map()->items, key, w1);
        if (val) free(val); // If something is already there, free it
    }
}

void add_plant(int x, int y)
{
    MapItem* w1 = (MapItem*) malloc(sizeof(MapItem));
    w1->type = PLANT;
    w1->draw = draw_plant;
    w1->walkable = true;
    w1->data = NULL;
    void* val = insertItem(get_active_map()->items, XY_KEY(x, y), w1);
    if (val) free(val); // If something is already there, free it
}

void add_debri(int x, int y)
{
    MapItem* w1 = (MapItem*) malloc(sizeof(MapItem));
    w1->type = DEBRI;
    w1->draw = draw_debri;
    w1->walkable = false;
    w1->data = NULL;
    void* val = insertItem(get_active_map()->items, XY_KEY(x, y), w1);
    if (val) free(val); // If something is already there, free it
}

void add_survivor(int x, int y)
{
    MapItem* w1 = (MapItem*) malloc(sizeof(MapItem));
    w1->type = SURVIVOR;
    w1->draw = draw_survivor;
    w1->walkable = false;
    w1->data = NULL;
    void* val = insertItem(get_active_map()->items, XY_KEY(x, y), w1);
    if (val) free(val); // If something is already there, free it
}

void add_door(int x, int y)
{
    MapItem* w1 = (MapItem*) malloc(sizeof(MapItem));
    w1->type = DOOR;
    w1->draw = draw_door;
    w1->walkable = false;
    w1->data = NULL;
    void* val = insertItem(get_active_map()->items, XY_KEY(x, y), w1);
    if (val) free(val); // If something is already there, free it
}

void add_medkit(int x, int y)
{
    MapItem* w1 = (MapItem*) malloc(sizeof(MapItem));
    w1->type = MEDKIT;
    w1->draw = draw_medkit;
    w1->walkable = false;
    w1->data = NULL;
    void* val = insertItem(get_active_map()->items, XY_KEY(x, y), w1);
    if (val) free(val); // If something is already there, free it
}

void add_axe(int x, int y)
{
    MapItem* w1 = (MapItem*) malloc(sizeof(MapItem));
    w1->type = AXE;
    w1->draw = draw_axe;
    w1->walkable = false;
    w1->data = NULL;
    void* val = insertItem(get_active_map()->items, XY_KEY(x, y), w1);
    if (val) free(val); // If something is already there, free it
}