#include "../lib.h"

void free_textures(t_mlx *mlx)
{
    if (mlx->maps.no_texture)
        mlx_destroy_image(mlx->mlx, mlx->maps.no_texture);
    if (mlx->maps.so_texture)
        mlx_destroy_image(mlx->mlx, mlx->maps.so_texture);
    if (mlx->maps.we_texture)
        mlx_destroy_image(mlx->mlx, mlx->maps.we_texture);
    if (mlx->maps.ea_texture)
        mlx_destroy_image(mlx->mlx, mlx->maps.ea_texture);
}
void load_texture(t_mlx *mlx, char *path, int index)
{
    void *img;
    int width;
    int height;
    
    img = mlx_xpm_file_to_image(mlx->mlx, path, &width, &height);
    if (!img)
    {
        // Handle error more gracefully
        printf("Error: Texture loading failed for %s\n", path);  // For debugging
        free_textures(mlx);
        return ;
    }
    mlx->add[index] = mlx_get_data_addr(img, &mlx->img.bits_per_pixel, 
                                               &mlx->img.line_length, &mlx->img.endian);
    mlx->width[index] = width;
    mlx->height[index] = height;
    
    // Check that texture data has been correctly loaded
    if (!mlx->add[index])
    {
        printf("Error: Texture data pointer is null for %s\n", path);
        free_textures(mlx);
    }
}
