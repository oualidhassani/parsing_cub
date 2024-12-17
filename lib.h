#ifndef CUB3D_H
#define CUB3D_H

#include <stdio.h>
#include <unistd.h>
#include "mlx/mlx.h"
#include <string.h>
#include <math.h>
#include <stdlib.h>
#include <stdbool.h>
#include <limits.h>
#include <float.h>
#include "parsing/parsing.h"
#define TILE_SIZE 32
#define MAP_WIDTH 7
#define MAP_HEIGHT 7
#define WINDOW_WIDTH 1920
#define WINDOW_HEIGHT 1280
#define TD_MAP_SIZE 7 * TILE_SIZE
#define EPSILON 0.0001
extern char *map[7];

typedef struct s_ray 
{
    int id;
    float ray_angle;
    float wall_hit_x;
    float wall_hit_y;
    int foundHorzWallHit;
    int foundVerticalHit;
    bool was_hit_vertical;
    bool was_hit_horizontal;
    float vertical_distance;
    float distance;
    float horizontal_distance;
    int is_ray_facing_up;
    int is_ray_facing_down;
    int is_ray_facing_right;
    int is_ray_facing_left;
    int wall_hit_content;
    float next_vertical_touch_x;
    float next_vertical_touch_y;
    float next_horizontal_touch_x;
    float next_horizontal_touch_y;
} t_ray;

typedef struct s_player
{
    float p_x;
    float p_y;
    float size;
    int   dir;
    float radius;
    float turn_direction;
    float walk_direction;
    float rotation_angle;
    float move_speed;
    float rotation_speed;
    float fov;
    float wall_strip_width;
    int   number_of_rays;
    float start_column_angle;
    t_ray *rays;
} t_player;

typedef struct s_rgb
{
    int r;
    int g;
    int b;
} t_rgb;

typedef struct s_img 
{
    void	*img;
	char	*addr;
	int		bits_per_pixel;
	int		line_length;
	int		endian;
} t_img;

typedef struct s_mlx 
{
    void *mlx;
    void *win;
    t_img img;
    t_player player;
} t_mlx  ;


void player_position(t_mlx *mlx);
void init_player(t_mlx *mlx);
void draw_player(t_mlx *mlx);
void my_mlx_pixel_put(t_img *img, int x, int y, int color);
void draw_line (t_mlx *mlx);
void update_player(t_mlx *mlx);
int does_hit_right_Bottom_wall(t_mlx *mlx, int x, int y);
void player_center_position(t_mlx *mlx, int x, int y);
void render_3D_projection_walls(t_mlx *mlx);
void draw_line_3D_helper(t_mlx *mlx, int x, int start_y, int end_y, int color);
void render_all(t_mlx *mlx);
#endif