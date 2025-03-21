#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <windows.h>
#include <math.h>
#include <dirent.h>


//2 COLLUM      NUMBER OF VOCALS
int number_of_vocals(FILE *file){
    int vocals = 0; //number of vocals
    int lines = 0; //number of lines in a file
    char c;
    //cycle that reads file character by character and counts lines - for a file to have a new line, there has to be a '\n'
    while ((c = fgetc(file)) != EOF){
        if (c == '\n'){
            lines++; //if it finds a '\n', increase number of lines
        }
    }
    //checks the last line of the file - file can end with '\n' (which is equivalent to Enter) but doesn't have to
    if (c != '\n' && lines > 0){
        lines++;
    }
    rewind(file);
    vocals = lines/2; //number of vocals = number of lines/2
    return vocals;
}

//3,6,9 COLLUM      NUMBER OF VOCALS
int vocal_sound (FILE *file, char p){
    int vocalUnder50 = 0;   //number of vocals under 50kHz
    int vocalOver50 = 0;    //number of vocals over 50kHz
    int vocalComplex = 0;   //number of complex vocals - vocals that have a number in their type-code
    char c = '\0';
    char c_prev = '\0';     //variable to store previous character
    double sound1 = 0;
    double sound2 = 0;
    int k = 0;              //variable to verify if a vocal is complex or not

    //cycle that reads file character by character
    while((c = fgetc(file)) != EOF){
            //complex vocals have codes that have numbers in the begining
            if(isalpha(c) && isdigit(c_prev)){
                vocalComplex++;
                k = 1;
            }
            //precaution for the typos (when someone writes 'd3' instead of '3d')
            if(isdigit(c) && isalpha(c_prev)){
                vocalComplex++;
                k = 1;
            } else if ((isalpha(c) == 0) && (c != '\n') && (c != '\\')){ //resets indicator k if it doesn't find a complex vocal
                k = 0;
            }
            c_prev = c; //remembers the previous character

            if ((c == '\\') && (k == 0)){ //when we get to the line with hertzes AND it isn't a complex vocal
                if ((fscanf(file, "\t%lf\t%lf\n", &sound1, &sound2) == 2)){ //scans the values from the line
                    //if a vocal is under or over 50kHz is verified by the placement of the middle of the vocal - if the middle is below 50kHz, then the
                    //majority of the vocal is below 50kHz (same for over 50kHz)
                    if (((sound1 + sound2)/2) < 50000){
                        vocalUnder50++;
                    }
                    if(((sound1 + sound2)/2) > 50000){
                        vocalOver50++;
                    }
                }
            }
    }
    rewind(file);
    //function returns one value at a time - we decide which value it is by parameter p
    if(p == 'u'){
        return vocalUnder50;
    }
    if(p == 'o'){
        return vocalOver50;
    }
    if(p == 'c'){
        return vocalComplex;
    } else {
        return 0;
    }
}

//4,7,10 COLLUM      CENTRE OF THE RANGE
double centre_range(FILE *file, int number_of_vocals, char p){
    char c = '\0';
    char c_prev = '\0';
    double sound1;
    double sound2;

    double centre_range_under = 0.000000;
    double centre_range_over = 0.000000;
    double centre_range_complex = 0.000000;

    int k = 0;
    while((c = fgetc(file)) != EOF){
        if(isalpha(c) && isdigit(c_prev)){
                k = 1;
            }
            if(isdigit(c) && isalpha(c_prev)){
                k = 1;
            } else if ((isalpha(c) == 0) && (c != '\n') && (c != '\\')){
                k = 0;
            }
            c_prev = c;

        if (c == '\\'){
            if(k == 0){
                if(fscanf(file, "\t%lf\t%lf\n", &sound1, &sound2) == 2){
                    if(((sound1 + sound2)/2) < 50000){
                        centre_range_under = centre_range_under + (sound1 + sound2)/2;
                    }
                    if(((sound1 + sound2)/2) > 50000){
                        centre_range_over = centre_range_over + (sound1 + sound2)/2;
                    }
                }
            }
            if(k == 1){
                if(fscanf(file, "\t%lf\t%lf\n", &sound1, &sound2) == 2){
                    centre_range_complex = centre_range_complex + (sound1 + sound2)/2;
                }
            }
        }
    }
    rewind(file);
    if (number_of_vocals != 0){
        if(p == 'u'){
            centre_range_under = centre_range_under/number_of_vocals;
            return centre_range_under;
        }
        if(p == 'o'){
            centre_range_over = centre_range_over/number_of_vocals;
            return centre_range_over;
        }
        if(p == 'c'){
            centre_range_complex = centre_range_complex/number_of_vocals;
            return centre_range_complex;
        }
    }
    return 0;
}

//5,8,11 COLLUM      SIZE OF THE RANGE
double size_range(FILE *file, int number_of_vocals, char p){
    char c = '\0';
    char c_prev = '\0';

    double sound1;
    double sound2;

    double size_range_under = 0.000000;
    double size_range_over = 0.000000;
    double size_range_complex = 0.000000;

    int k = 0;
    while((c = fgetc(file)) != EOF){
        if(isalpha(c) && isdigit(c_prev)){
                k = 1;
            }
            if(isdigit(c) && isalpha(c_prev)){
                k = 1;
            } else if ((isalpha(c) == 0) && (c != '\n') && (c != '\\')){
                k = 0;
            }
            c_prev = c;

        if (c == '\\'){
            if(k == 0){
                if(fscanf(file, "\t%lf\t%lf\n", &sound1, &sound2) == 2){
                    if(((sound1 + sound2)/2) < 50000){
                        size_range_under = size_range_under + (sound2 - sound1);
                    }
                    if(((sound1 + sound2)/2) > 50000){
                        size_range_over = size_range_over + (sound2 - sound1);
                    }
                }
            }
            if(k == 1){
                if(fscanf(file, "\t%lf\t%lf\n", &sound1, &sound2) == 2){
                    size_range_complex = size_range_complex + (sound2 - sound1);
                }
            }
        }
    }
    rewind(file);
    if(number_of_vocals != 0){
        if(p == 'u'){
            size_range_under = size_range_under/number_of_vocals;
            return size_range_under;
        }
        if(p == 'o'){
            size_range_over = size_range_over/number_of_vocals;
            return size_range_over;
        }
        if(p == 'c'){
            size_range_complex = size_range_complex/number_of_vocals;
            return size_range_complex;
        }
    }
    return 0;
}

//12,13 COLLUM     NUMBER OF BOUTS
int bouts(FILE *file, char p){
    char line[260];
    int bouts = 0;
    int calls = 0;

    double time1;
    double time2;
    double time_prev = -0.2;

    int chain = 0;
    while (fgets(line, sizeof(line), file)) {
        if (line[0] != '\\') {
            if (sscanf(line, "%lf %lf", &time1, &time2) == 2) {
                if(time1 - time_prev < 0.2){
                    calls++;
                    if(chain == 0){
                        bouts++;
                        chain = 1;
                    }
                } else {
                    chain = 0;
                }
                time_prev = time2;
            }
        }
    }
    rewind(file);
    if(p == 'b'){
        return bouts;
    }
    if(p == 'c'){
        calls = calls + bouts;
        return calls;
    }
    return 0;
}

//14,15 COLLUM BOUTS OVER 50/UNDER 50
int bout_sound(FILE *file, char p){
    char line[260];

    double time1, time2;
    double time_prev = -0.2;
    double sound1, sound2;
    double sound1_prev = 0;
    double sound2_prev = 0;

    double bout_middle = 0;
    double bouts_centre = 0;

    int chain = 1;
    int boutOver50 = 0;
    int boutUnder50 = 0;

    while(fgets(line, sizeof(line), file)){
        sscanf(line, "%lf %lf", &time1, &time2);
        fscanf(file, "\\%lf\t%lf\n", &sound1, &sound2);

        if(chain == 1){
            bout_middle = (sound1_prev + sound2_prev)/2;
        }
        if(time1 - time_prev < 0.2){
            bout_middle = bout_middle + (sound1 + sound2)/2;
            chain++;
        } else if(chain != 1){
            bouts_centre = bout_middle/chain;
            if(bouts_centre > 50000){
                boutOver50++;
            }
            if(bouts_centre < 50000){
                boutUnder50++;
            }
            bout_middle = 0;
            chain = 1;
        }
        time_prev = time2;
        sound1_prev = sound1;
        sound2_prev = sound2;
    }
    if(chain != 1){
        bouts_centre = bout_middle/chain;
        if(bouts_centre > 50000){
            boutOver50++;
        }
        if(bouts_centre < 50000){
            boutUnder50++;
        }
    }
    rewind(file);
    if(p == 'u'){
        return boutUnder50;
    }
    if(p == 'o'){
        return boutOver50;
    }
    return 0;
}

//16, 17, 18 COLLUM ISOLATED CALLS
int isolated_calls(FILE *file, char p){
    char line[260];
    double time1_next, time2_next;
    double sound1_next, sound2_next;

    double time2_prev = 0;
    double time1 = 0;
    double time2 = 0;
    double sound1 = 0;
    double sound2 = 0;

    int isocalls = 0;
    double iso_middle = 0;
    int isoUnder = 0;
    int isoOver = 0;
    while(fgets(line, sizeof(line), file)){
        sscanf(line, "%lf %lf", &time1_next, &time2_next);
        fscanf(file, "\\%lf\t%lf\n", &sound1_next, &sound2_next);

        if((time1 - time2_prev > 0.2) && (time1_next - time2 > 0.2)){
            isocalls++;
            iso_middle = (sound1 + sound2)/2;
            if(iso_middle < 50000){
                isoUnder++;
            }
            if(iso_middle > 50000){
                isoOver++;
            }
        }
        time2_prev = time2;
        time1 = time1_next;
        time2 = time2_next;
        sound1 = sound1_next;
        sound2 = sound2_next;
    }
    if(time1 - time2_prev > 0.2){
        isocalls++;
        iso_middle = (sound1 + sound2)/2;
        if(iso_middle < 50000){
            isoUnder++;
        }
        if(iso_middle > 50000){
            isoOver++;
        }
    }
    rewind(file);
    if(p == '#'){
        return isocalls;
    }
    if(p == 'u'){
        return isoUnder;
    }
    if(p == 'o'){
        return isoOver;
    }
    return 0;
}

void output(FILE *fp, FILE *file, const char *filename){
    if (file != NULL){
        int vocalisations = number_of_vocals(file);//COLLUM 2
        int vocalsUnder50 = vocal_sound(file, 'u');//COLLUM 3
        double centre_rangeUnder50 = centre_range(file, vocalsUnder50, 'u');//COLLUM 4
        double size_rangeUnder50 = size_range(file, vocalsUnder50, 'u');//COLLUM 5
        int vocalsOver50 = vocal_sound(file, 'o');//COLLUM 6
        double centre_rangeOver50 = centre_range(file, vocalsOver50, 'o');//COLLUM 7
        double size_rangeOver50 = size_range(file, vocalsOver50, 'o');//COLLUM 8
        int vocalsComplex = vocal_sound(file, 'c');//COLLUM 9
        double centre_rangeComplex = centre_range(file, vocalsComplex, 'c');//COLLUM 10
        double size_rangeComplex = size_range(file, vocalsComplex, 'c');//COLLUM 11
        int bout = bouts(file, 'b');//COLLUM 12
        double calls = bouts(file, 'c');//COLLUM 13
            if(bout != 0){
                calls = calls/bout;
            }
        int boutUnder50 = bout_sound(file, 'u');//COLLUM 14
        int boutOver50 = bout_sound(file, 'o');//COLLUM 15
        int isocalls = isolated_calls(file, '#');//COLLUM 16
        int isoUnder50 = isolated_calls(file, 'u');//COLLUM 17
        int isoOver50 = isolated_calls(file, 'o');//COLLUM 18

        fprintf(fp, "%s;%d;%d;%lf;%lf;%d;%lf;%lf;%d;%lf;%lf;%d;%d;%d;%d;%d;%d;%d\n", filename, vocalisations, vocalsUnder50, centre_rangeUnder50,size_rangeUnder50,
                vocalsOver50, centre_rangeOver50, size_rangeOver50, vocalsComplex, centre_rangeComplex, size_rangeComplex, bout, ((int)floor(calls + 0.5)),
                boutUnder50, boutOver50, isocalls, isoUnder50, isoOver50);

        fclose(file);

    }
}

int main()
{
    //name of the excel file
    FILE *fp = fopen("excelOutput.csv", "w");
    //names of the collums in the excel file
    fprintf(fp, "SUBJECT;VOCALS;VOCALS UNDER 50kHz;MIDDLE RANGE;SIZE RANGE;VOCALS OVER 50kHz;MIDDLE RANGE;SIZE RANGE;COMPLEX VOCALS;MIDDLE RANGE;SIZE RANGE;BOUTS;"
                "AVERAGE CALLS IN BOUT;BOUTS UNDER 50kHz;BOUTS OVER 50kHz;ISOLATED CALLS;ISOCALLS UNDER 50kHz;ISOCALLS OVER 50kHz\n\n");

    char filename[50];
    struct dirent *entry;
    DIR *dp = opendir(".");

    if (dp == NULL) {
        printf("Chyba: Nepodarilo sa otvoriù prieËinok.\n");
        return 1;
    }

    while ((entry = readdir(dp))) {
        if (strstr(entry->d_name, ".txt") != NULL) {
            snprintf(filename, sizeof(filename), "%s", entry->d_name);
            // Pok˙sime sa otvoriù tento s˙bor
            FILE *file = fopen(filename, "r");
            output(fp, file, filename);
        }
    }

    fclose(fp);
    return 0;
}
