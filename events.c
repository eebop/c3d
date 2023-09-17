#include <SDL2/SDL.h>
#include "gfx.h"
#include "quaternion.h"

void update_debug(SDL_Event event, scene *s) {
    quaternion qin;
    quaternion qtemp;
    quaternion qout;
    double i;
    double j;
    double k;
    if (event.type == SDL_KEYDOWN) {
        switch (event.key.keysym.sym) {
        case SDLK_w:
            i++;
            break;
        case SDLK_s:
            i--;
            break;
        case SDLK_q:
            j++;
            break;
        case SDLK_e:
            j--;
            break;
        case SDLK_d:
            k++;
            break;
        case SDLK_a:
            k--;
            break;
        case SDLK_LEFT:
            submitRotation(s, 1, 1);
            break;
        case SDLK_RIGHT:
            submitRotation(s, 1, 0);
            break;
        case SDLK_DOWN:
            submitRotation(s, 2, 1);
            break;
        case SDLK_UP:
            submitRotation(s, 2, 0);
            break;
        case SDLK_k:
            s->settings->useArctan = 0;
            break;
        default:
            printf("unknown key: %d\n", event.key.keysym.sym);
            break;
        }
    }
    if (event.type == SDL_MOUSEBUTTONUP) {
        switch (event.button.button)
        {
        case SDL_BUTTON_X1:
            submitRotation(s, 0, 0);
            break;
        case SDL_BUTTON_X2:
            submitRotation(s, 0, 1);
            break;
        default:
            break;
        }
    }
    if (event.type == SDL_KEYUP) {
        switch (event.key.keysym.sym) {
            case SDLK_k:
                s->settings->useArctan = 1;
                break;
            default:
                break;
        }
    }
    CREATE_QUATERNION(qin, i, j, k);
    multiplyWithInverseFirstQuaternion(s->c->q, &qin, &qtemp);
    multiplyQuaternion(&qtemp, s->c->q, &qout);

    s->c->cx += qout.i;
    s->c->cy += qout.j;
    s->c->cz += qout.k;

}