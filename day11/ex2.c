#include <stdio.h>
#include <stdlib.h>

#include "map.h"

int main()
{	
	char title_palette[] = {'.','#','@'};
	
	_S_MAP_OBJECT test_map;
	map_init(&test_map);
	_S_MAP_OBJECT test_map2;
	map_init(&test_map2);

	map_new(&test_map,8,8);
	map_dump(&test_map,title_palette);
	//test_map.nwe(8,8)

	map_new(&test_map2,5,5);
	map_dump(&test_map2,title_palette);

	return 0;
}
