// Include standard input/output header
#include <stdio.h>
// Include necessary header files
#include <string.h>

int main() {

    // *** INPUT VARIABLES ***
    char team[51];
    char opp[51]; 
    int games        = 12;
    int week         = 1;  
    int pts          = 0;
    int drives       = 0;
    int yards        = 0;
    int plays        = 0;
    int mins         = 0;

    // *** CALCULATED VARIABLES ***
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

    // *** AUTOMATIC CSV HEADER CHECK ***
    FILE *check_file = fopen("final_tv.csv", "r");
    
    // If the file doesn't exist, create it and write the headers
    if (check_file == NULL) {
        FILE *init_file = fopen("final_tv.csv", "w");
        if (init_file != NULL) {
            fprintf(init_file, "Week,Team,Opponent,Points,Drives,Yards,Plays,Minutes,PPD,YPP,PPM,Threat Value,TVD\n");
            fclose(init_file);
        }
    } else {
        fclose(check_file);
    }

    // *** INPUT FROM USER ***
    printf("Enter team name: \n");
    fgets(team, sizeof(team), stdin);
    team[strcspn(team, "\n")] = 0;

    printf("How many games did %s play? \n", team);
    scanf("%d", &games);
    getchar(); // Consume newline

    // Open the CSV file in append mode before the loop begins
    FILE *file = fopen("final_tv.csv", "a");

    for (int i = 1; i <= games; i++) {

        printf("\n*------ Game %d ------*\n", i);

        printf("What week number was Game %d? \n", i);
        scanf("%d", &week);
        getchar();

        printf("Who was the opponent for Game %d? \n", i);
        fgets(opp, sizeof(opp), stdin);
        opp[strcspn(opp, "\n")] = 0;

        printf("How many points did %s score? \n", team);
        scanf("%d", &pts);
        getchar();
        tot_pts += pts;

        printf("How many drives did %s have? \n", team);
        scanf("%d", &drives);
        getchar();
        tot_drives += drives;

        printf("How many yards did %s gain? \n", team);
        scanf("%d", &yards);
        getchar();
        tot_yards += yards;

        printf("How many plays did %s run? \n", team);
        scanf("%d", &plays);
        getchar();
        tot_plays += plays;

        printf("How many minutes did %s have the ball? \n", team);
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

        // Print results for this specific game to the console
        printf(">> GAME %d RESULTS -> PPD: %.2f | YPP: %.2f | PPM: %.2f | THREAT VALUE: %.2f (TVD: %.2f)\n", i, ppd, ypp, ppm, tv, tv_diff);

        // Automatically writes *every* game to the CSV sequentially inside the loop ---
        if (file != NULL) {
            fprintf(file, "%d,%s,%s,%d,%d,%d,%d,%d,%.2f,%.2f,%.2f,%.2f,%.2f\n", 
                week, team, opp, pts, drives, yards, plays, mins, ppd, ypp, ppm, tv, tv_diff);
            printf(">> Game %d logged to final_tv.csv.\n", i);
        }
    }

    // Close the append stream safely after logging all games
    if (file != NULL) {
        fclose(file);
    }

    // *** OVERALL SEASON STATS ***
    if (tot_drives > 0 && tot_plays > 0 && tot_mins > 0) {
        ppd = (float)tot_pts / tot_drives;
        ypp = (float)tot_yards / tot_plays;
        ppm = (float)tot_plays / tot_mins;
        tv  = ppd * ypp * ppm;
    }

    // Calculate correct average season differential
    if (games > 0) {
        avg_tvd = tot_tvd_running / games;
    }

    // *** OUTPUT FINAL SEASON REPORT TO CONSOLE ***
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

    return 0;
}
