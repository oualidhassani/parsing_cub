#ifndef PARSING_H
#define PARSING_H
#include <fcntl.h>


typedef struct s_map
{
    char    *no_texture;
    char    *so_texture;
    char    *we_texture;
    char    *ea_texture;
    char    *floor_color;
    char   *ceiling_color;
    char	**map;
    struct s_player player;
    struct s_rgb    floor_rgb;
    

} t_map;








#endif