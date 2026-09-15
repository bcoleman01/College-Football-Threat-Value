// Include standard input/output header
#include <stdio.h>
// Include necessary header files
#include <string.h>

int main() {

    // *** INPUT VARIABLES ***
    // Allocated memory brackets so it registers as a full text string
    char team[51];
    int games        = 12;
    int pts          = 0;
    int drives       = 0;
    int yards        = 0;
    int plays        = 0;
    int mins         = 0;

    // *** CALCULATED VARIABLES ***
    // Variables to hold current game inputs
    float ppd          = 0.0;
    float ypp          = 0.0;
    float ppm          = 0.0;
    float tv           = 0.0;
    float tv_diff      = 0.0;
    float avg_tvd      = 0.0;

    // Running totals to aggregate season stats
    int tot_pts    = 0;
    int tot_drives = 0;
    int tot_yards  = 0;
    int tot_plays  = 0;
    int tot_mins   = 0;
    float tot_tvd_running = 0.0; 

    // *** INPUT FROM USER ***
    // Prompt user for team name
    printf("Enter team name: \n");
    // Read team name from input
    fgets(team, sizeof(team), stdin);
    // Remove newline character from the input
    team[strcspn(team, "\n")] = 0;

    // Prompt user for number of games played
    printf("How many games did %s play? \n", team);
    // Read number of games played from input
    scanf("%d", &games);
    // Consume the newline character left in the input buffer
    getchar();

    for (int i = 1; i <= games; i++) {

        printf("\n*------ Game %d ------*\n", i);

        // Prompt user for points scored
        printf("How many points did %s score in Game %d? \n", team, i);
        scanf("%d", &pts);
        getchar();
        tot_pts += pts;

        // Prompt user for drives
        printf("How many drives did %s have in Game %d? \n", team, i);
        scanf("%d", &drives);
        getchar();
        tot_drives += drives;

        // Prompt user for yards gained
        printf("How many yards did %s gain in Game %d? \n", team, i);
        scanf("%d", &yards);
        getchar();
        tot_yards += yards;

        // Prompt user for plays ran
        printf("How many plays did %s run in Game %d? \n", team, i);
        scanf("%d", &plays);
        getchar();
        tot_plays += plays;

        // Prompt user for time of possession
        printf("How many minutes did %s have the ball in Game %d? \n", team, i);
        scanf("%d", &mins);
        getchar();
        tot_mins += mins;

        // *** PER-GAME CALCULATIONS ***
        if (drives > 0 && plays > 0 && mins > 0) {
            ppd = (float)pts / drives;
            ypp = (float)yards / plays;
            ppm = (float)plays / mins;
            tv  = ppd * ypp * ppm;
            tv_diff = tv - pts;
            tot_tvd_running += tv_diff; 
        } else {
            ppd = ypp = ppm = tv = tv_diff = 0.0;
        }

        // Print results for this specific game to the terminal window
        printf(">> GAME %d RESULTS -> PPD: %.2f | YPP: %.2f | PPM: %.2f | THREAT VALUE: %.2f (TVD: %.2f)\n", i, ppd, ypp, ppm, tv, tv_diff);
    }

    // *** OVERALL SEASON STATS ***
    if (tot_drives > 0 && tot_plays > 0 && tot_mins > 0) {
        ppd = (float)tot_pts / tot_drives;
        ypp = (float)tot_yards / tot_plays;
        ppm = (float)tot_plays / tot_mins;
        tv  = ppd * ypp * ppm;
    }

    // Calculate the accurate cumulative season average differential
    if (games > 0) {
        avg_tvd = tot_tvd_running / games;
    }

    // *** OUTPUT FINAL SEASON REPORT TO TERMINAL ***
    printf("\n*=====================================*\n");
    printf("  %s FINAL SEASON REPORT\n", team);
    printf("*=====================================*\n");
    printf("Season Points Per Drive (PPD):   %.2f\n", ppd);
    printf("Season Yards Per Play (YPP):     %.2f\n", ypp);
    printf("Season Plays Per Minute (PPM):   %.2f\n", ppm);
    printf("---------------------------------------\n");
    printf("FINAL SEASON THREAT VALUE (TV):  %.2f\n", tv);
    printf("AVERAGE THREAT VALUE DIFFERENTIAL (Avg. TVD):  %.2f\n", avg_tvd);
    printf("*=====================================*\n");

    // *** GENERATE THE TEXT FILE ***
    FILE *txt_file = fopen("final_tv.txt", "w");
    if (txt_file != NULL) {
        fprintf(txt_file, "*=====================================*\n");
        fprintf(txt_file, "  %s FINAL SEASON REPORT\n", team);
        fprintf(txt_file, "*=====================================*\n");
        fprintf(txt_file, "Season Points Per Drive (PPD):   %.2f\n", ppd);
        fprintf(txt_file, "Season Yards Per Play (YPP):     %.2f\n", ypp);
        fprintf(txt_file, "Season Plays Per Minute (PPM):   %.2f\n", ppm);
        fprintf(txt_file, "---------------------------------------\n");
        fprintf(txt_file, "FINAL SEASON THREAT VALUE (TV):  %.2f\n", tv);
        fprintf(txt_file, "AVERAGE THREAT VALUE DIFFERENTIAL (Avg. TVD):  %.2f\n", avg_tvd);
        fprintf(txt_file, "*=====================================*\n");
        fclose(txt_file);
        printf("\n>> Summary report successfully exported to 'final_tv.txt'.\n");
    }

    return 0;
}
