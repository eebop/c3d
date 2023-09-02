#include <SDL2/SDL.h>
#include <math.h>
#include "gfx.h"

scene *alloc_scene(void)
{
    scene *s = (scene *)malloc(sizeof(scene));
    s->c = (camera *)malloc(sizeof(camera));
    s->c->cx = 0.0;
    s->c->cy = 0.0;
    s->c->cz = 0.0;
    s->c->a1 = 0;
    s->c->a2 = 0;
    s->c->fov = 50;
    s->points = (point3 **) malloc(64 * sizeof(point3 *));
    s->max_points = 64;
    s->num_points = 0;
    s->textures = (texture **) malloc(64 * sizeof(texture *));
    s->max_textures = 8;
    s->num_textures = 0;
    for (int i = 0; i != 64; i++)
    {
        s->points[i] = NULL;
    }
    for (int j = 0; j != 8; j++)
    {
        s->points[j] = NULL;
    }
    s->settings = (scene_settings *) malloc(sizeof(scene_settings));
    s->settings->useArctan = 1;

    return s;
}

void submit_txt(scene *s, texture *t)
{
    if (s->num_textures == s->max_textures)
    {
        s->textures = realloc(s->textures, 2 * s->max_textures * sizeof(texture *));
        s->max_textures *= 2;
    }
    t->p[0] += s->num_points;
    t->p[1] += s->num_points;
    t->p[2] += s->num_points;
    t->p[3] += s->num_points;

    s->textures[s->num_textures] = t;
    s->num_textures++;
}

void submit_pt(scene *s, point3 *p)
{

    if (s->num_points == s->max_points)
    {
        s->points = realloc(s->points, 2 * s->max_points * sizeof(point3 *));
        s->max_points *= 2;
    }
    s->points[s->num_points] = p;
    s->num_points++;
}

int compute_one(point3 *p, scene *s, SDL_FPoint *op, SDL_Point *jp)
{
    double x = p->x - s->c->cx;
    double y = p->y - s->c->cy;
    double z = p->z - s->c->cz;
    double cos1 = SDL_cos(s->c->a1);
    double cos2 = SDL_cos(s->c->a2);
    double sin1 = SDL_sin(s->c->a1);
    double sin2 = SDL_sin(s->c->a2);
    double outx;
    double outy;
    double outz;
    double angle1;
    double angle2;
    outz = x * sin1 + z * cos1;
       x = x * cos1 - z * sin1;

    outy = x * sin2 - y * cos2;
    outx = x * cos2 + y * sin2;
    if (s->settings->useArctan) {
        angle1 = SDL_atan2(outz, outx);
        angle2 = SDL_atan2(outy, outx);
    } else {
        angle1 = outz / outx;
        angle2 = outy / outx;
    }
    if (angle1 > M_PI_2 || angle1 < -M_PI_2 || angle2 > M_PI_2 || angle2 < -M_PI_2 || outx < 0)
    {
        // This texture is behind the camera, so we don't need to render it
        // This assumes the fov is less than 180
        return -1;
    }
    // SDL uses both points and float points for some reason, so we have to create both
    if (op != NULL)
    {
        op->x = angle1 * 180 * 800 / (s->c->fov * M_PI) + 400;
        op->y = angle2 * 180 * 800 / (s->c->fov * M_PI) + 400;
    }
    if (jp != NULL)
    {
        jp->x = angle1 * 180 * 800 / (s->c->fov * M_PI) + 400;
        jp->y = angle2 * 180 * 800 / (s->c->fov * M_PI) + 400;
    }
    return 0;
}

unsigned int lt(scene *s, texture *t1, texture *t2)
{
    double x1 = s->points[t1->p[0]]->x + s->points[t1->p[1]]->x + s->points[t1->p[2]]->x + s->points[t1->p[3]]->x - (4 * s->c->cx);
    double y1 = s->points[t1->p[0]]->y + s->points[t1->p[1]]->y + s->points[t1->p[2]]->y + s->points[t1->p[3]]->y - (4 * s->c->cy);
    double z1 = s->points[t1->p[0]]->z + s->points[t1->p[1]]->z + s->points[t1->p[2]]->z + s->points[t1->p[3]]->z - (4 * s->c->cz);
    double x2 = s->points[t2->p[0]]->x + s->points[t2->p[1]]->x + s->points[t2->p[2]]->x + s->points[t2->p[3]]->x - (4 * s->c->cx);
    double y2 = s->points[t2->p[0]]->y + s->points[t2->p[1]]->y + s->points[t2->p[2]]->y + s->points[t2->p[3]]->y - (4 * s->c->cy);
    double z2 = s->points[t2->p[0]]->z + s->points[t2->p[1]]->z + s->points[t2->p[2]]->z + s->points[t2->p[3]]->z - (4 * s->c->cz);
    return ((x1 * x1) + (y1 * y1) + (z1 * z1) > (x2 * x2) + (y2 * y2) + (z2 * z2));
}

texture **_sort(scene *s, texture **t, unsigned int numtex)
{
    if (numtex == 0) {
        // This should never happen but if somehow sort gets called with 0 elements throw a warning and then exit the function
        printf("Warning: _sort() called with n=0; returning t\n");
        return t;
    }
    texture **pt = malloc(numtex * sizeof(texture *));
    if (numtex == 1)
    {
        pt[0] = t[0];
        return pt;
    }
    if (numtex == 2)
    {
        if (lt(s, t[0], t[1]))
        {
            pt[0] = t[0];
            pt[1] = t[1];
        }
        else
        {
            pt[0] = t[1];
            pt[1] = t[0];
        }
        return pt;
    }
    texture **h1 = _sort(s, t, numtex / 2);
    texture **h2 = _sort(s, t + (numtex / 2), numtex - (numtex / 2));
    unsigned int i = 0;
    unsigned int j = 0;
    unsigned int im = numtex / 2;
    unsigned int jm = numtex - (numtex / 2);
    for (int k = 0; k != numtex; k++)
    {

        if (i == im)
        {
            pt[k] = h2[j++];
            continue;
        }
        if (j == jm)
        {
            pt[k] = h1[i++];
            continue;
        }
        if (lt(s, h1[i], h2[j]))
        {
            pt[k] = h1[i++];
        }
        else
        {
            pt[k] = h2[j++];
        }
    }
    free(h1);
    free(h2);
    return pt;
}

void compileScene(scene *s)
{
    // sort the textures so that the farthest away ones get rendered first
    // this way overlaps look right
    if (s->num_textures == 0)
    {
        return;
    }
    texture **t = s->textures;
    s->textures = _sort(s, s->textures, s->num_textures);
    free(t);
}

void render(SDL_Renderer *r, scene *s)
{
    SDL_Vertex v[6];
    SDL_Point p[5];
    for (int k = 0; k != 6; k++)
    {
        v[k].tex_coord = (SDL_FPoint){0.5, 0.5};
    }
    for (int i = 0; i != s->num_textures; i++)
    {
        for (int k = 0; k!=6;k++) {
            v[k].color = s->textures[i]->c;
        }
        for (int j = 0; j != 4; j++)
        {

            if (compute_one(s->points[s->textures[i]->p[j]], s, &v[j].position, &p[j]))
            {
                goto end;
            }
        }
        v[4] = v[0];
        v[5] = v[2];
        p[4] = p[0];
        SDL_RenderGeometry(r, NULL, v, 6, NULL, 0);
        SDL_SetRenderDrawColor(r, 0x00, 0xFF, 0x00, 0xFF);
        SDL_RenderDrawLines(r, p, 5);
        end:
    }
}
