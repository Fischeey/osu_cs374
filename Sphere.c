#include <stdio.h>
#include <math.h>

void main(){
    float pi = 3.14159265359;

    //user input for the number of spheres to be entered
    int respN = -1;
    while (respN < 2 || respN > 10){
        printf("How many spherical segments you want to evaluate [2-10]?");
        scanf("%d", &respN);
    }

    //decalre values to be summed up for the eventual average
    float sumSA = 0;
    float sumVOL = 0;

    //runs the sphere data gathering and calculation for the amount entered previousely 
    for (int i = 1; i <= respN; i++){
        //bool variable, i looked up the syntax but it seemed more complicated to me then doing this with an int
        int validEntry = 0;
        while (validEntry==0){

            //gets the three values for the calculation
            float respR;
            printf("What is the radius of the sphere (R)? \n");
            scanf("%f", &respR);

            float respHA;
            printf("What is the height of the top area of the spherical segment (ha)? \n");
            scanf("%f", &respHA);

            float respHB;
            printf("What is the height of the bottom area of the spherical segment (hb)? \n");
            scanf("%f", &respHB);

            printf("Entered Data: R = %f HA =  %f HB = %f\n", respR, respHA, respHB);

            //if any of the following get triggered then one of the values is incorrect, split up to work better in my head

            if ( respR < 0 || respHA < 0 || respHB < 0 ){
                printf("Invalid Input. restarting for current sphere\n");
            } else if (respHA > respR || respHB > respR){
                printf("Invalid Input. restarting for current sphere\n");
            } else if (respHA < respHB){
                printf("Invalid Input. restarting for current sphere\n");
            } else {
                //variables good, proceed with calculation
                validEntry = 1;
                float a = sqrt((respR*respR - respHA * respHA));
                float b = sqrt((respR*respR - respHB * respHB));
                float h = respHA - respHB;
                float VOL = ((float)1/6) * pi * h * (3 * a * a + 3 * b * b + h * h);
                float SA = (pi * a * a) + (pi * b * b) + (2 * pi * respR * h);
                
                printf("Total Surface Area = %f Volume  =  %f ", SA, VOL);
                sumSA += SA;
                sumVOL += VOL;
            }
        }
    }
    printf("Average Surface Area = %f Average Volume  =  %f \n", sumSA/respN, sumVOL/respN);
}