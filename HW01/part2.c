#include <stdio.h>

int main()
{
    float diameter;
    float radius;
    float PI = 3.14;
    float area_of_sphere;

    printf("Enter the diameter of a circle: ");
    scanf("%f", &diameter);

    radius = diameter / 2;
    area_of_sphere = 4 * PI * radius * radius;

    printf("\nr = %.1f\n", radius);
    printf("\nArea of sphere: %.2f\n", area_of_sphere);

    return 0;
}
