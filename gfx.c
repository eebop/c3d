#include <SDL2/SDL.h>
#include <math.h>
#include "main.h"
#include "gfx.h"
#include "quaternion.h"


static quaternion rotations[6] = {
    {0.9996988186962042, -0.024541228522912288, 0, 0},
    {0.9996988186962042, 0, -0.024541228522912288, 0},
    {0.9996988186962042, 0, 0, -0.024541228522912288},
    {0.9996988186962042,  0.024541228522912288, 0, 0},
    {0.9996988186962042,  0, 0.024541228522912288, 0},
    {0.9996988186962042,  0, 0, 0.024541228522912288}
};

scene *alloc_scene(void)
{
    scene *s = (scene *)malloc(sizeof(scene));
    s->c = (camera *)malloc(sizeof(camera));
    s->c->cx = 0.0;
    s->c->cy = 0.0;
    s->c->cz = 0.0;
    s->c->q = malloc(sizeof(quaternion));
    s->c->q->i = 0;
    s->c->q->j = 0;
    s->c->q->k = 0;
    s->c->q->t = 1;
    s->points = (quaternion **) malloc(64 * sizeof(quaternion *));
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
    s->settings->useArctan = 0;
    s->settings->fov = 50;
    s->settings->grabMouse = 0;
    return s;
}

void submitQuaternionRotation(scene *s, quaternion *rotation) {
    multiplyQuaternion(rotation, s->c->q, s->c->q);
}

void submitRotation(scene *s, int rotation, int direction) {
    submitQuaternionRotation(s, &(rotations[rotation + (3 * direction)]));
}

void submit_txt(scene *s, texture *t)
{
    if (s->num_textures == s->max_textures)
    {
        s->textures = realloc(s->textures, 2 * s->max_textures * sizeof(texture *));
        s->max_textures *= 2;
    }
    if (t->type == FOUR_POINTS) {
        t->p[0] += s->num_points;
        t->p[1] += s->num_points;
        t->p[2] += s->num_points;
        t->p[3] += s->num_points;
    }
    if (t->type == ONE_POINT) {
        t->p[0] += s->num_points;
    }

    s->textures[s->num_textures] = t;
    s->num_textures++;
}

void submit_pt(scene *s, quaternion *p)
{

    if (s->num_points == s->max_points)
    {
        s->points = realloc(s->points, 2 * s->max_points * sizeof(quaternion *));
        s->max_points *= 2;
    }
    s->points[s->num_points] = p;
    s->num_points++;
}

int compute_one(quaternion *p, scene *s, SDL_FPoint *op, SDL_Point *jp)
{
    quaternion qin;
    quaternion qout;
    quaternion qtemp;
    CREATE_QUATERNION(qin, p->i - s->c->cx, p->j - s->c->cy, p->k - s->c->cz);
    multiplyQuaternion(s->c->q, &qin, &qtemp);
    multiplyWithInverseSecondQuaternion(&qtemp, s->c->q, &qout);
    double outx = qout.i;
    double outy = qout.j;
    double outz = qout.k;
    double angle1;
    double angle2;
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
        op->x = angle1 * 180 * 800 / (s->settings->fov * M_PI) + 400;
        op->y = angle2 * 180 * 800 / (s->settings->fov * M_PI) + 400;
    }
    if (jp != NULL)
    {
        jp->x = angle1 * 180 * 800 / (s->settings->fov * M_PI) + 400;
        jp->y = angle2 * 180 * 800 / (s->settings->fov * M_PI) + 400;
    }
    return 0;
}

unsigned int lt(scene *s, texture *t1, texture *t2)
{
    if (t1->front) {
        return 0;
    }
    if (t2->front) {
        return 1;
    }
    double x1 = s->points[t1->p[0]]->i + s->points[t1->p[1]]->i + s->points[t1->p[2]]->i + s->points[t1->p[3]]->i - (4 * s->c->cx);
    double y1 = s->points[t1->p[0]]->j + s->points[t1->p[1]]->j + s->points[t1->p[2]]->j + s->points[t1->p[3]]->j - (4 * s->c->cy);
    double z1 = s->points[t1->p[0]]->k + s->points[t1->p[1]]->k + s->points[t1->p[2]]->k + s->points[t1->p[3]]->k - (4 * s->c->cz);
    double x2 = s->points[t2->p[0]]->i + s->points[t2->p[1]]->i + s->points[t2->p[2]]->i + s->points[t2->p[3]]->i - (4 * s->c->cx);
    double y2 = s->points[t2->p[0]]->j + s->points[t2->p[1]]->j + s->points[t2->p[2]]->j + s->points[t2->p[3]]->j - (4 * s->c->cy);
    double z2 = s->points[t2->p[0]]->k + s->points[t2->p[1]]->k + s->points[t2->p[2]]->k + s->points[t2->p[3]]->k - (4 * s->c->cz);
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
    for (unsigned int k = 0; k != numtex; k++)
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


void DrawCircle(SDL_Renderer* renderer, int32_t centreX, int32_t centreY, int32_t radius)
    {
    const int32_t diameter = (radius * 2);

    int32_t x = (radius - 1);
    int32_t y = 0;
    int32_t tx = 1;
    int32_t ty = 1;
    int32_t error = (tx - diameter);

    while (x >= y)
    {
    // Each of the following renders an octant of the circle
    SDL_RenderDrawPoint(renderer, centreX + x, centreY - y);
    SDL_RenderDrawPoint(renderer, centreX + x, centreY + y);
    SDL_RenderDrawPoint(renderer, centreX - x, centreY - y);
    SDL_RenderDrawPoint(renderer, centreX - x, centreY + y);
    SDL_RenderDrawPoint(renderer, centreX + y, centreY - x);
    SDL_RenderDrawPoint(renderer, centreX + y, centreY + x);
    SDL_RenderDrawPoint(renderer, centreX - y, centreY - x);
    SDL_RenderDrawPoint(renderer, centreX - y, centreY + x);

      if (error <= 0)
      {
      	++y;
      	error += ty;
      	ty += 2;
      }

      if (error > 0)
      {
      	--x;
      	tx += 2;
      	error += (tx - diameter);
      }

    }
}

void render(SDL_Renderer *r, scene *s)
{
    SDL_Vertex v[6];
    SDL_Point p[5];
    for (int k = 0; k != 6; k++)
    {
        v[k].tex_coord = (SDL_FPoint){0.5, 0.5};
    }
    for (unsigned int i = 0; i != s->num_textures; i++)
    {
        if (s->textures[i]->type == FOUR_POINTS) {
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

        }
        if (s->textures[i]->type == ONE_POINT) {
            if (compute_one(s->points[s->textures[i]->p[0]], s, &v[0].position, &p[0])) {
                goto end;
            }
            
            SDL_SetRenderDrawColor(r, 0x00, 0x00, 0xFF, 0xFF);
            DrawCircle(r, p[0].x, p[0].y, 2);
            //SDL_FillRect(r, &re, 0x9090AFF);
        }
        end:;

    }
}
