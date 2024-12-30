#include "../lib.h"

float normalize_angle(float angle) {
    angle = remainder(angle, 2 * M_PI);
    if (angle < 0) {
        angle += (2 * M_PI);
    }
    return angle;
}

void ray (t_mlx *mlx, t_ray *ray)
{
    ray->ray_angle = normalize_angle(mlx->player.start_column_angle);
    ray->wall_hit_x = 0;
    ray->wall_hit_y = 0;
    ray->distance = 0;
    ray->vertical_distance = 0;
    ray->horizontal_distance = 0;
    ray->is_ray_facing_down = ray->ray_angle > 0 && ray->ray_angle <  M_PI; //condition
    ray->is_ray_facing_up = !ray->is_ray_facing_down;
    ray->is_ray_facing_right = ray->ray_angle < 0.5 * M_PI || ray->ray_angle > 1.5 * M_PI;
    ray->is_ray_facing_left = !ray->is_ray_facing_right;
    ray->was_hit_vertical = false;
}

void ray_init (t_mlx *mlx)
{
    int i;

    i = 0;
    while(i < mlx->player.number_of_rays)
    {
        ray(mlx, &mlx->player.rays[i]);
        i++;
    }
}

void init_player(t_mlx *mlx)
{
    player_position(mlx);
    mlx->player.size = 4;
    mlx->player.radius = 3;
    mlx->player.turn_direction = 0;
    mlx->player.walk_direction = 0;
    mlx->player.rotation_angle = M_PI/2;
    mlx->player.move_speed = 3.0;
    mlx->player.rotation_speed = 2 * (M_PI / 180); //formula to get radius angle from degrees
    mlx->player.fov = 60 * (M_PI / 180);
    mlx->player.wall_strip_width = 1;
    mlx->player.number_of_rays = WINDOW_WIDTH/mlx->player.wall_strip_width;
    mlx->player.rays = malloc(sizeof(t_ray) * mlx->player.number_of_rays);
    if (!mlx->player.rays)
        exit(1);
    mlx->player.start_column_angle = 0;
    ray_init (mlx);
}

int map_has_wall_at(t_mlx *mlx, float x, float y) {
    if (x < 0 || x >= mlx->maps.width * TILE_SIZE || y < 0 || y >= mlx->maps.height * TILE_SIZE) {
        return 1; // Outside bounds, treat as a wall
    }
    int mapGridIndexX = floor(x / TILE_SIZE);
    int mapGridIndexY = floor(y / TILE_SIZE);
    
    // Ensure indices are within the bounds of the map array
    if (mapGridIndexX < 0 || mapGridIndexX >= mlx->maps.height || 
        mapGridIndexY < 0 || mapGridIndexY >= mlx->maps.width) {
        return 1; // Treat as a wall if out of bounds
    }
    
    return mlx->maps.map[mapGridIndexY][mapGridIndexX] == '1'; // Check bounds here
}

int does_hit_right_Bottom_wall(t_mlx *mlx, int x, int y)
{
    int right_x = x + (mlx->player.size/2);
    int bottom_y = y + (mlx->player.size/2);
    int converting_x_to_grid = floor(right_x / TILE_SIZE);
    int converting_y_to_grid = floor(bottom_y / TILE_SIZE);
    
    if(mlx->maps.map[converting_y_to_grid][converting_x_to_grid] == '1')
    {
        printf("oups !! you're hitting a wall\n");
        return (1);
    }
    return 0;
}

int does_hit_left_top_wall(t_mlx *mlx, int x, int y)
{
    int left_x = x + (mlx->player.size/-2);
    int top_y = y + (mlx->player.size/-2);
    int converting_x_to_grid = floor(left_x / TILE_SIZE);
    int converting_y_to_grid = floor(top_y / TILE_SIZE);

    if(mlx->maps.map[converting_y_to_grid][converting_x_to_grid] == '1')
    {
        return (1);
    }
    return 0;
}

void update_player(t_mlx *mlx)
{
    int old_x = mlx->player.p_x;
    int old_y = mlx->player.p_y;
    float move_step = 0;
    float tmp_walk_direction = mlx->player.walk_direction;
    float tmp_turn_direction = mlx->player.turn_direction;
    mlx->player.turn_direction = 0;
    mlx->player.walk_direction = 0;
    // Accumulate the rotation angle over time
    mlx->player.rotation_angle += tmp_turn_direction * mlx->player.rotation_speed;
    move_step = tmp_walk_direction * mlx->player.move_speed;
    mlx->player.p_x += cos(mlx->player.rotation_angle) * move_step;
    mlx->player.p_y += sin(mlx->player.rotation_angle) * move_step;
    if(does_hit_right_Bottom_wall(mlx ,mlx->player.p_x, mlx->player.p_y) || 
        does_hit_left_top_wall(mlx, mlx->player.p_x, mlx->player.p_y))
    {
        mlx->player.p_x = old_x;
        mlx->player.p_y = old_y;
    }
}

int apply_shading(int base_color, float shading_factor)
{
    int r;
    int g;
    int b;
    int result;

    r = (base_color >> 16) & 0xFF;
    g = (base_color >> 8) & 0xFF;
    b = base_color & 0xFF;

    r = (int)(r * shading_factor);
    g = (int)(g * shading_factor);
    b = (int)(b * shading_factor);

    if (r > 255) r = 255;
    if (g > 255) g = 255;
    if (b > 255) b = 255;
    if (r < 0) r = 0;
    if (g < 0) g = 0;
    if (b < 0) b = 0;

    result = (r << 16) | (g << 8) | b;
    return result;
}

// void draw_line_3D_helper(t_mlx *mlx, int x, int start_y, int end_y, int color)
// {
//     if (start_y > end_y)
//         return;

//     for (int y = start_y; y < end_y; y++)
//     {
//         //mlx_destroy_image(mlx->mlx, mlx->img.img);
//         my_mlx_pixel_put(&mlx->img, x, y, color);
//     }
// }
void draw_line_3D_helper(t_mlx *mlx, int x, int start_y, int end_y, int color)
{
    for (int y = start_y; y < end_y; y++)
    {
        my_mlx_pixel_put(&mlx->img, x, y, color);
    }
}

int get_texture_color(t_mlx *mlx, int texture_x, int texture_y, int texture_index)
{
    if (texture_x < 0 || texture_y < 0 || 
        texture_x >= mlx->width[texture_index] || 
        texture_y >= mlx->height[texture_index] || 
        !mlx->add[texture_index])
        return 0;

    texture_x = texture_x % mlx->width[texture_index];
    texture_y = texture_y % mlx->height[texture_index];
    
    return mlx->add[texture_index][texture_y * mlx->width[texture_index] + texture_x];
}

void render_3D_projection_walls(t_mlx *mlx)
{
    for (int x = 0; x < mlx->player.number_of_rays; x++)
    {
        t_ray *ray = &mlx->player.rays[x];
        
        double perpWallDist = ray->distance;
        if (perpWallDist <= 0.0) perpWallDist = 0.1;

        int lineHeight = (int)(WINDOW_HEIGHT / perpWallDist * TILE_SIZE);

        int drawStart = -lineHeight / 2 + WINDOW_HEIGHT / 2;
        int drawEnd = lineHeight / 2 + WINDOW_HEIGHT / 2;
        if (drawStart < 0) drawStart = 0;
        if (drawEnd >= WINDOW_HEIGHT) drawEnd = WINDOW_HEIGHT - 1;

        int texNum;
        if (ray->was_hit_vertical) 
        {
            texNum = ray->is_ray_facing_left ? 0 : 1; 
        } else 
        {
            texNum = ray->is_ray_facing_up ? 2 : 3;   
        }

        double wallX;
        if (ray->was_hit_vertical)
            wallX = ray->wall_hit_y - floor(ray->wall_hit_y / TILE_SIZE) * TILE_SIZE;
        else
            wallX = ray->wall_hit_x - floor(ray->wall_hit_x / TILE_SIZE) * TILE_SIZE;

        // Get texture X coordinate
        int texX = (int)(wallX * mlx->width[texNum] / TILE_SIZE);
        if (ray->was_hit_vertical && ray->is_ray_facing_left)
            texX = mlx->width[texNum] - texX - 1;
        if (!ray->was_hit_vertical && ray->is_ray_facing_up)
            texX = mlx->width[texNum] - texX - 1;

        double step = 1.0 * mlx->height[texNum] / lineHeight;
        double texPos = (drawStart - WINDOW_HEIGHT / 2 + lineHeight / 2) * step;

        // Draw ceiling
        for (int y = 0; y < drawStart; y++)
            my_mlx_pixel_put(&mlx->img, x, y, 0x87CEEB);

        // Draw wall
        for (int y = drawStart; y < drawEnd; y++)
        {
            int texY = (int)texPos;
            if (texY >= mlx->height[texNum]) texY = mlx->height[texNum] - 1;
            texPos += step;
            
            int color = get_texture_color(mlx, texX, texY, texNum);
            if (ray->was_hit_vertical)
                color = (color >> 1) & 8355711;  // Apply shading for vertical walls

            my_mlx_pixel_put(&mlx->img, x, y, color);
        }

        // Draw floor
        for (int y = drawEnd; y < WINDOW_HEIGHT; y++)
            my_mlx_pixel_put(&mlx->img, x, y, 0x8B4513);
    }
}







// Helper function to draw vertical lines

void draw_ray_line(t_mlx *mlx, float x1, float y1, float x2, float y2, int color)
{
    float dx = x2 - x1;
    float dy = y2 - y1;
    float step;
    (void)mlx;
    (void)color;
    if (fabs(dx) > fabs(dy))
        step = fabs(dx);
    else
        step = fabs(dy);
    
    float x_inc = dx / step;
    float y_inc = dy / step;
    
    float x = x1;
    float y = y1;
    
    for (int i = 0; i <= step; i++)
    {
        x += x_inc;
        y += y_inc;
    }
}

void vertical_ray_intersection(t_mlx *mlx, t_ray *ray)
{
    float xintercept;
    float yintercept;
    float xstep;
    float ystep;
    float next_vertical_touch_x;
    float next_vertical_touch_y;
    ray->next_vertical_touch_x = 0;
    ray->next_vertical_touch_y = 0;
    int found_vertical_wall_hit = false;
    xintercept = floor(mlx->player.p_x / TILE_SIZE) * TILE_SIZE;
    if (ray->is_ray_facing_right)
        xintercept += TILE_SIZE;

    yintercept = mlx->player.p_y + (xintercept - mlx->player.p_x) * tan(ray->ray_angle);

    xstep = TILE_SIZE;
    if (ray->is_ray_facing_left)
        xstep *= -1;

    ystep = TILE_SIZE * tan(ray->ray_angle);
    if (ray->is_ray_facing_up && ystep > 0)
        ystep *= -1;
    if (ray->is_ray_facing_down && ystep < 0)
        ystep *= -1;
    next_vertical_touch_x = xintercept;
    next_vertical_touch_y = yintercept;
    if (ray->is_ray_facing_left)
        next_vertical_touch_x = xintercept - EPSILON;
    while(next_vertical_touch_x >= 0 && 
       next_vertical_touch_x <= mlx->maps.width * TILE_SIZE && 
       next_vertical_touch_y >= 0 && 
       next_vertical_touch_y <= mlx->maps.height * TILE_SIZE)
    {
        if(map_has_wall_at(mlx, next_vertical_touch_x, next_vertical_touch_y))
        {
            found_vertical_wall_hit = 1;
            break;
        }
        else
        {
            // Increment 'i' based on the distance
            next_vertical_touch_x += xstep;
            next_vertical_touch_y += ystep;
        }
    }
    ray->foundVerticalHit = found_vertical_wall_hit;
    if (found_vertical_wall_hit)
    {
        float distance = sqrt(pow(next_vertical_touch_x - mlx->player.p_x, 2) + pow(next_vertical_touch_y - mlx->player.p_y, 2));
        ray->vertical_distance = distance;
    }
    else
        ray->vertical_distance = FLT_MAX;
    ray->next_vertical_touch_x = next_vertical_touch_x;
    ray->next_vertical_touch_y = next_vertical_touch_y;
}
void horizontal_line_intersection(t_mlx *mlx, t_ray *ray)
{
    float xintercept;
    float yintercept;
    float xstep;
    float ystep;
    float next_horizontal_touch_x = 0;
    float next_horizontal_touch_y = 0;
    int found_horizontal_wall_hit = 0;
    yintercept = floor(mlx->player.p_y / TILE_SIZE) * TILE_SIZE;
    if (ray->is_ray_facing_down)
        yintercept += TILE_SIZE;
    xintercept = mlx->player.p_x + (yintercept - mlx->player.p_y) / tan(ray->ray_angle);
    ystep = TILE_SIZE;
    if (ray->is_ray_facing_up)
        ystep *= -1;

    xstep = TILE_SIZE / tan(ray->ray_angle);
    if (ray->is_ray_facing_left && xstep > 0)
        xstep *= -1;
    if (ray->is_ray_facing_right && xstep < 0)
        xstep *= -1;

    next_horizontal_touch_x = xintercept;
    next_horizontal_touch_y = yintercept;

    if (ray->is_ray_facing_up)
        next_horizontal_touch_y = yintercept - EPSILON; // Move slightly up
    // printf("ray angle = %f\n", ray->ray_angle);
    // printf("next horizontal touch = %f\n", next_horizontal_touch_x);
    // printf("map width => %d\n",mlx->maps.width);
    while(next_horizontal_touch_x >= 0 && 
       next_horizontal_touch_x <= mlx->maps.width * TILE_SIZE && 
       next_horizontal_touch_y >= 0 && 
       next_horizontal_touch_y <= mlx->maps.height * TILE_SIZE)
    {
        if(map_has_wall_at(mlx, next_horizontal_touch_x, next_horizontal_touch_y))
        {
            found_horizontal_wall_hit = 1;
            break;
        }
        else
        {
            next_horizontal_touch_x += xstep;
            next_horizontal_touch_y += ystep;
        }
    }
    ray->foundHorzWallHit = found_horizontal_wall_hit;
    if (found_horizontal_wall_hit)
    {
        float distance = sqrt(pow(next_horizontal_touch_x - mlx->player.p_x, 2) + pow(next_horizontal_touch_y - mlx->player.p_y, 2));
        ray->horizontal_distance = distance;
    }
    else
        ray->horizontal_distance = FLT_MAX;
    ray->next_horizontal_touch_x = next_horizontal_touch_x;
    ray->next_horizontal_touch_y = next_horizontal_touch_y;
}

void cast (t_mlx *mlx, t_ray *ray)
{
    vertical_ray_intersection(mlx, ray);
    horizontal_line_intersection(mlx, ray);
    if (ray->horizontal_distance < ray->vertical_distance)
    {
        ray->wall_hit_x = ray->next_horizontal_touch_x;
        ray->wall_hit_y = ray->next_horizontal_touch_y;
        ray->distance = ray->horizontal_distance;
        ray->was_hit_vertical = true;
    }
    else 
    {
        ray->wall_hit_x = ray->next_vertical_touch_x;
        ray->wall_hit_y = ray->next_vertical_touch_y;
        ray->distance = ray->vertical_distance;
        ray->was_hit_vertical = false;
    }
    //draw_ray_line(mlx, mlx->player.p_x, mlx->player.p_y, ray->wall_hit_x, ray->wall_hit_y, 0x00eeeee4);
}
void adjusting_rays(t_mlx *mlx)
{
    int column = 0;
    mlx->player.start_column_angle = mlx->player.rotation_angle - (mlx->player.fov / 2);
    mlx->player.start_column_angle = normalize_angle(mlx->player.start_column_angle);
    while(column < mlx->player.number_of_rays)
    {
        mlx->player.rays[column].id = column;
        mlx->player.rays[column].ray_angle = mlx->player.start_column_angle;
        ray(mlx, &mlx->player.rays[column]);
        mlx->player.start_column_angle += mlx->player.fov/mlx->player.number_of_rays;
        mlx->player.start_column_angle = normalize_angle(mlx->player.start_column_angle);
        cast(mlx, &mlx->player.rays[column]);
        column++;
    }
}

void draw_line (t_mlx *mlx)
{
    int i = 0;
    float x = 0;
    float y = 0;
    float angle = mlx->player.rotation_angle;
    while(i < 30)
    {
        x = mlx->player.p_x + i * cos(angle);
        y = mlx->player.p_y + i * sin(angle);
        if (x >= 0 && x < mlx->maps.td_map_size && y >= 0 && x < mlx->maps.td_map_size)
        {
            my_mlx_pixel_put(&mlx->img, (int)x, (int)y, 0x00eeeee4);  
        }
        i++;
    }
    // draw_simple_rays(mlx);
}
void player_center_position(t_mlx *mlx, int x, int y)
{
    mlx->player.p_x = x * TILE_SIZE + (TILE_SIZE / 2);
    mlx->player.p_y = y * TILE_SIZE + (TILE_SIZE / 2);
}

void draw_player(t_mlx *mlx)
{
    int i = 0;
    int j = 0;

    int top_left_x = mlx->player.p_x - (mlx->player.size / 2); //added these to center the shit out of the player
    int top_left_y = mlx->player.p_y - (mlx->player.size / 2);
    while(i < mlx->player.size)
    {
        j = 0;
        while(j < mlx->player.size)
        {
            my_mlx_pixel_put(&mlx->img, j + top_left_x, i + top_left_y, 0x00ffff00);
            j++;
        }
        i++;
    }
    draw_line(mlx);
}
void render_all(t_mlx *mlx)
{
    adjusting_rays(mlx);
    render_3D_projection_walls(mlx);
}