#include <stdio.h>
#include <math.h>
#include <string.h>
#include <unistd.h>

char pixel_values[] =".,-~:;=!*#$@";
#define MAX_X_HEIGHT 80
#define MAX_Y_HEIGHT 25
#define TOTAL_PIXELS 2000
#define MATH_PI      3.14
int radius_of_circle    = 1;
int offset_from_y_axis  = 2;
int K2                  = 5;
float z_buffer[TOTAL_PIXELS];
char illumination[TOTAL_PIXELS];

/*The pixels plotted using the values above in the illumination array are
meant to demonstrate illumination. It starts with the dimmest
and ends with the highest.*/
int main(){
    /*ANSI/VT100 terminal control escape sequence. Clear the screen and set cursor to home*/
    printf("\x1b[2J");
    int x = 1;
    float A = 0;
    float B = 0;
    float omega = 0;
    float phi   = 0;
    while(x){
        memset(illumination,32,TOTAL_PIXELS*sizeof(char));
        memset(z_buffer,0,TOTAL_PIXELS*sizeof(float));
        for(omega = 0; omega < 2 * MATH_PI; omega += 0.07){
            for(phi = 0; phi < 2 * MATH_PI; phi += 0.02){
                float cosA = cos(A);
                float sinA = sin(A);
                float cosB = cos(B);
                float sinB = sin(B);
                float sinomega = sin(omega);
                float cosomega = cos(omega);
                float sinphi = sin(phi);
                float cosphi = cos(phi);
                
                float calc1 = offset_from_y_axis + radius_of_circle*cosomega;
                /*3D coordinates*/
                float xprime = calc1*(cosB*cosphi + sinA*sinB*sinphi) - radius_of_circle*cosA*sinB*sinomega;
                float yprime = calc1*(cosphi*sinB - cosB*sinA*sinphi) + radius_of_circle*cosA*cosB*sinomega;
                
                float inverse_zprime = 1/(calc1*cosA*sinphi + radius_of_circle*sinA*sinomega + K2);
                /*2D coordinates*/
                int x = 40 + 30*xprime*inverse_zprime;
                int y = 12 + 15*yprime*inverse_zprime;
                /*Location of while pixel, the x and y coordinates correspond to.*/
                int pixel_location = x + MAX_X_HEIGHT*y;
                /*Illumination calculation, The illlumination is normailized by 8 to evenly spread across 12 ascii characters*/
                int Normalized_illumination = 8*(cosphi*cosomega*sinB - cosA*cosomega*sinphi - sinA*sinomega + cosB*(cosA*sinomega-cosomega*sinA*sinphi));
                Normalized_illumination = (Normalized_illumination >0) ? Normalized_illumination : 0;
                if(x<MAX_X_HEIGHT && x> 0 && y < MAX_Y_HEIGHT && y>0 && inverse_zprime>z_buffer[pixel_location] && Normalized_illumination <13){
                    z_buffer[pixel_location]        = inverse_zprime;
                    illumination[pixel_location]    = pixel_values[Normalized_illumination];
                }
            }
        }
        /*ANSI/VT100 terminal control escape sequence. Return cursor to home position*/
        printf("\x1b[H");
        for(int i = 0; i <TOTAL_PIXELS + 1; i++){
            /*Every 80 Characters print ASCII code 10 which is Line Feed*/
            switch(illumination[i]){
                case '@':
                case '$':
                case '#':
                    printf("\x1B[33m");
                    break;
                case '*':
                case '!':
                case '=':
                    printf("\x1B[32m");
                    break;
                case ';':
                case ':':
                case '~':
                    printf("\x1B[31m");
                    break;
                case  '-':
                case  ',':
                case  '.':
                    printf("\x1B[34m");
                    break;
                default:
                    break;
            }
            putchar(i % MAX_X_HEIGHT ? illumination[i] : 10);
            printf("\x1B[0m"); // Seets colour to default.
            A += 0.00004;
            B += 0.00002;
        }
        usleep(30000);
    }
    return 0;
}
