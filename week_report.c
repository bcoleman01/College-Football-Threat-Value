// Include standard input/output header
#include <stdio.h>
// Include necessary header files
#include <string.h>

int main() {

    // INPUT VARIABLES
    char team[50];
    int week         = 0;
    char opp[50];
    int pts          = 0;
    int drives       = 0;
    int yards        = 0;
    int plays        = 0;
    int mins         = 0;


    // CALCULATED VARIABLES
    float ppd          = 0.0;
    float ypp          = 0.0;
    float ppm          = 0.0;
    float tv           = 0.0;
    float tv_diff      = 0.0;

    // LOOP VARIABLE
    char choice        = 'n';

    do {
        // INPUT FROM USER
        printf("Enter week number: \n");
        scanf(" %d", &week);
        getchar();

        printf("Enter team name: \n");
        fgets(team, sizeof(team), stdin);
        team[strcspn(team, "\n")] = 0;

        printf("Enter opponent name: \n");
        fgets(opp, sizeof(opp), stdin);
        opp[strcspn(opp, "\n")] = 0;

        printf("\n*------ Week %d: %s vs %s ------*\n", week, team, opp);

        printf("How many points did %s score? \n", team);
        scanf("%d", &pts); getchar();

        printf("How many drives did %s have? \n", team);
        scanf("%d", &drives); getchar();

        printf("How many yards did %s gain? \n", team);
        scanf("%d", &yards); getchar();

        printf("How many plays did %s run? \n", team);
        scanf("%d", &plays); getchar();

        printf("How many minutes did %s have the ball? \n", team);
        scanf("%d", &mins); getchar();


        // GAME CALCULATIONS
        if (drives > 0 && plays > 0 && mins > 0) {
            ppd = (float)pts / drives;
            ypp = (float)yards / plays;
            ppm = (float)plays / mins;
            tv  = ppd * ypp * ppm;
            
            // Percentage Variance Math: prevents 100+ scaling outliers while handling 0.0 edge cases
            if (tv > 0.0) {
                tv_diff = tv - pts;
            } else {
                tv_diff = 0.0;
            }
        } else {
            ppd = ypp = ppm = tv = tv_diff = 0.0;
        }

        // OUTPUT GAME REPORT
        printf("\n*=====================================*\n");
        printf("  %s GAME REPORT\n", team);
        printf("*=====================================*\n");
        printf("Points Per Drive (PPD):   %.2f\n", ppd);
        printf("Yards Per Play (YPP):     %.2f\n", ypp);
        printf("Plays Per Minute (PPM):   %.2f\n", ppm);
        printf("---------------------------------------\n");
        printf("THREAT VALUE (TV):        %.2f\n", tv);
        printf("THREAT DIFFERENTIAL (TVD): %.2f%\n", tv_diff); // Added % symbol for visual clarity
        printf("*=====================================*\n");

        // WRITE TO CSV
        FILE *file = fopen("26_single_tv.csv", "a");
        if (file != NULL) {
            fprintf(file, "%d,%s,%s,%d,%d,%d,%d,%d,%.2f,%.2f,%.2f,%.2f,%.2f\n", 
                week, team, opp, pts, drives, yards, plays, mins, ppd, ypp, ppm, tv, tv_diff);
            fclose(file);
            printf(">> Game logged to 26_single_tv.csv.\n");
        }

        // LOOP PROMPT
        printf("\nWould you like to enter another game? (y/n): ");
        scanf(" %c", &choice);
        getchar();

    } while (choice == 'y' || choice == 'Y');

    printf("\nExiting program. Entries saved successfully to 26_single_tv.csv\n");

    return 0;
}