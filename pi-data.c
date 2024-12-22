
#include "includes_global.h"
#include "pi-data.h"
#include "http-get.h"
#include "plot.h"
#include "llist.h"
#include "cJSON.h"
#include <time.h>

#include <string.h>

const char pass[] = "71ecc9be7159aa584124c920047dd84ccc2df6b6fb38a0ae6d54cdf61b06a0aa"; //this is the token you need to change (don't forget about the IPs below!)

int getSummary(char *resp)
{
    char addr[1000] = "http://192.168.50.3/admin/api.php?summary&auth=";  //also change the IP of the piHole with your piHole's IP
    strcat(addr, pass);
     fprintf(stderr,"For GET: \n%s\n\n", addr);
    http_get_response_t *res = http_get(addr);

    if (res == 0)
    {
        return -1;
    }

    if (res->ok == 0)
    {
        http_get_free(res);
        return -2;
    }

    if (res->status != 200)
    {
        fprintf(stderr, "Status:%ld\n", res->status);
        http_get_free(res);
        return -3;
    }

    if (res->data)
    {
        strcpy(resp, res->data);
        printf("Summary:::::\n%s\n", res->data);
        http_get_free(res);
        return 0;
    }
    else
    {
        http_get_free(res);
        return -4;
    }
}

int getOver10Mins(char *resp)
{

    char addr[1000] = "http://192.168.50.3/admin/api.php?overTimeData10mins&auth="; //change the IP here as well !!!
    strcat(addr, pass);
    // fprintf(stderr,"%s\n\n", addr);

    http_get_response_t *res = http_get(addr);

    if (res == 0)
    {
        return -1;
    }

    if (res->ok == 0)
    {
        http_get_free(res);
        return -2;
    }

    if (res->status != 200)
    {
        fprintf(stderr, "Status:%ld\n", res->status);
        http_get_free(res);
        return -3;
    }

    if (res->data)
    {
        strcpy(resp, res->data);
        http_get_free(res);
        return 0;
    }
    else
    {
        http_get_free(res);
        return -4;
    }
}

int getMaxValueFromArray(long int *array, int size)
{
    int max = SDL_MIN_SINT32;
    while (size--)
    {
        if (*array > max)
        {
            max = *array;
        }
        // fprintf(stderr,"== max=%d    *array=%d -> %d\n", max, *array, max);

        array++;
    }
    return max;
}

inline void drawPlotOver10Mins(SDL_Window *window, SDL_Renderer *renderer, char *resp)
{
#define MAX_NO_OF_POINTS 5000
    uint32_t no_of_points = 0;

    cJSON *overt10_json;
    cJSON *element;
    long int times[MAX_NO_OF_POINTS], values[MAX_NO_OF_POINTS], i;

    if (!resp)
        return;
    overt10_json = cJSON_Parse(resp);
    if (overt10_json == NULL)
    {
        const char *error_ptr = cJSON_GetErrorPtr();
        if (error_ptr != NULL)
        {
            fprintf(stderr, "Error before: %s\n", error_ptr);
        }
    }

    // all ok
    const cJSON *dom_over_time = NULL;
    dom_over_time = cJSON_GetObjectItemCaseSensitive(overt10_json, "domains_over_time");
    if (cJSON_IsObject(dom_over_time))
    {
        fprintf(stderr,"Checking dom_over_time \n");
        char *jsn;
        jsn = cJSON_Print(dom_over_time);
        if (jsn)
        {
            // fprintf(stderr,"%s\n", jsn);
            free((void *)jsn);
        }

        fprintf(stderr,"Parsing json to array\n");
        // for (i = 0; i < NO_OF_POINTS; i++)
        //{
        while (element = cJSON_GetArrayItem(dom_over_time, no_of_points))
        {
            if (element)
            {
                // fprintf(stderr,"%s %d %d\n", element->string, element->valueint, no_of_points );
                times[no_of_points] = atoi(element->string);
                values[no_of_points] = element->valueint;
                no_of_points++;
                // fprintf(stderr,"%d %d \n", times[i],values[i]);
            }
            else
            {
                fprintf(stderr,"no %d is invalid element\n", no_of_points);
                break;
            }
        }
    }
    else
        return;
    if (dom_over_time)
        free((void *)dom_over_time);

    // populate caption list
    captionlist caption_list = NULL;

    caption_list = push_back_caption(caption_list, "Queries over last 24h", 0, 0x00FFFF);
    // caption_list = push_back_caption(caption_list, "Upload", 1, 0xFF0000);

    // print_list_caption(caption_list);

    // populate coordinate list
    coordlist coordinate_list = NULL;

    // populate plot parameter object
    plot_params params;

    for (i = 0; i < no_of_points; i++)
    {
        char buf[100];
        struct tm ts;
        ts = *localtime(&times[i]);
        strftime(buf, sizeof(buf), "%H:%M", &ts);
        // fprintf(stderr,"%s\n", buf);
        coordinate_list = push_back_coord(coordinate_list, 0, i, values[i]);
        // fprintf(stderr,"Pushing %d at %s\n", values[i], buf);
        strcpy(params.graduation_x_text[i], buf);
    }

    // get max of values to set scale properly
    int max_val = getMaxValueFromArray(values, no_of_points);
    fprintf(stderr,"MAX VAL = %d\n", max_val);

    // print_list_coord(coordinate_list);

    params.screen_width = WINDOW_SIZE_W;
    params.screen_heigth = WINDOW_SIZE_H;
    params.plot_window_title = "plot-sdl";
    params.font_text_path = "./NovaMono.ttf";
    params.font_text_path_for_big_text = "hacker.otf";
    params.font_text_size = 14;
    params.font_text_size_for_big_text = 54;
    params.caption_text_x = "Time of day";
    params.caption_text_y = "number of Queries";
    params.caption_list = caption_list;
    params.coordinate_list = coordinate_list;
    params.scale_x = 1;
    params.scale_y = 100;
    params.max_x = no_of_points - 1;
    params.max_y = max_val + 20; // 1200;
    params.pos_x = 70;
    params.pos_y = 240;
    params.plot_h = 500;
    params.plot_w = 1500;

    plot_graph(&params, window, renderer);
    SDL_RenderPresent(renderer); // copy to screen
}

int getJsonValueInt(cJSON *root, char *ident)
{
    const cJSON *branch = NULL;
    char *jsn;
    branch = cJSON_GetObjectItem(root, ident);
    fprintf(stderr,"Checking %s \n", ident);
    int ret_val = 0;

    jsn = cJSON_Print(branch);
    if (jsn)
    {
        ret_val = (int)cJSON_GetNumberValue(branch);
        fprintf(stderr,"%s ==-> %d\n", (char *)jsn, ret_val);
        cJSON_free(jsn);
    }

    return ret_val;
}

float getJsonValueFloat(cJSON *root, char *ident)
{
    const cJSON *branch = NULL;
    char *jsn;
    branch = cJSON_GetObjectItem(root, ident);
    fprintf(stderr,"Checking %s \n", ident);
    float ret_val = 0;

    jsn = cJSON_Print(branch);
    if (jsn)
    {
        fprintf(stderr,"%s\n", (char *)jsn);
        ret_val = atof((char *)jsn);
        cJSON_free(jsn);
    }

    return ret_val;
}

void getJsonValueString(cJSON *root, char *ident, char *ret_val)
{
    cJSON *branch = NULL;
    char *jsn;
    branch = cJSON_GetObjectItem(root, ident);
    fprintf(stderr,"Checking %s \n", ident);

    jsn = cJSON_Print(branch);
    if (jsn)
    {
        if (ret_val)
        {
            char *temp_ret_val = cJSON_GetStringValue(branch);
            strcpy(ret_val, temp_ret_val);
        }
        fprintf(stderr,"%s ==-> %s\n", (char *)jsn, ret_val);
        cJSON_free(jsn);
    }
}

void drawSummaryText(SDL_Window *window, SDL_Renderer *renderer, char *text, char *val, int x, int y)
{

    char font_text_path_for_big_text[] = "hacker.otf";
    int font_text_size_for_big_text = 34;
    int font_text_size_for_small_text = 18;

    if (!text)
    {
        fprintf(stderr,"text is invalid pointer.\n");
        return;
    }

    if (!val)
    {
        fprintf(stderr,"val is invalid pointer.\n");
        return;
    }

    TTF_Font *smallFont = TTF_OpenFont(font_text_path_for_big_text, font_text_size_for_small_text);
    TTF_Font *valueFont = TTF_OpenFont(font_text_path_for_big_text, font_text_size_for_big_text);

    if (!smallFont || !valueFont)
    {
        fprintf(stderr,"TTF_OpenFont messed up opening the fonts. Skipping this time.\n");
        return;
    }

    SDL_Color font_color_small = {0, 180, 0};
    SDL_Color font_color_big = {0, 240, 0};

    char val_to_display[100];
    if (strcasestr(val, "percentage"))
        sprintf(val_to_display, "%s%%", val); // adding percentage sign to text
    else
        sprintf(val_to_display, "%s", val);

    SDL_Surface *valText = TTF_RenderText_Blended(valueFont, val_to_display, font_color_big);
    // SDL_Surface *valText = TTF_RenderText_Blended(valueFont, val, font_color_big);
    SDL_Surface *Text = TTF_RenderText_Blended(smallFont, text, font_color_small);

    SDL_Rect text_text;
    SDL_Texture *texture = SDL_CreateTextureFromSurface(renderer, Text);
    SDL_QueryTexture(texture, NULL, NULL, &text_text.w, &text_text.h);
    text_text.x = x; // params->plot_w / 2 - text_caption_x.w / 2;
    text_text.y = y; // plot_position_y + plot_heigth + 3 * regular_caption_text_heigth;
    SDL_RenderCopy(renderer, texture, NULL, &text_text);

    SDL_DestroyTexture(texture);

    texture = SDL_CreateTextureFromSurface(renderer, valText);
    SDL_QueryTexture(texture, NULL, NULL, &text_text.w, &text_text.h);
    text_text.x = x;      // params->plot_w / 2 - text_caption_x.w / 2;
    text_text.y = y + 30; // plot_position_y + plot_heigth + 3 * regular_caption_text_heigth;
    SDL_RenderCopy(renderer, texture, NULL, &text_text);

    SDL_DestroyTexture(texture);

    SDL_FreeSurface(valText);
    SDL_FreeSurface(Text);

    TTF_CloseFont(smallFont);
    TTF_CloseFont(valueFont);
}

void drawSummary(SDL_Window *window, SDL_Renderer *renderer, char *summary_json)
{

    cJSON *summary_json_parse;
    summary_json_parse = cJSON_Parse(summary_json);
    if (0 == summary_json_parse)
    {
        fprintf(stderr,"failed to parse summary!\n");
        return;
    }

    char *jsn_master;
    jsn_master = cJSON_Print(summary_json_parse);
    if (jsn_master)
    {
        fprintf(stderr,"%s\n", (char *)jsn_master);
        cJSON_free(jsn_master);
    }

    char domains_being_blocked[1000];
    getJsonValueString(summary_json_parse, "domains_being_blocked", domains_being_blocked);

    char dns_queries_today[1000];
    getJsonValueString(summary_json_parse, "dns_queries_today", dns_queries_today);

    char ads_blocked_today[1000];
    getJsonValueString(summary_json_parse, "ads_blocked_today", ads_blocked_today);

    char ads_percentage_today[1000];
    getJsonValueString(summary_json_parse, "ads_percentage_today", ads_percentage_today);

    cJSON_free(summary_json_parse);

    int summary_pos_x = 10;
    int summary_pos_y = 10;

    drawSummaryText(window, renderer, "Domains being blocked", domains_being_blocked, summary_pos_x + 20, summary_pos_y + 20);
    drawSummaryText(window, renderer, "DNS queries today", dns_queries_today, summary_pos_x + 420, summary_pos_y + 20);
    drawSummaryText(window, renderer, "Ads blocked today", ads_blocked_today, summary_pos_x + 720, summary_pos_y + 20);
    drawSummaryText(window, renderer, "Ads percentage today", ads_percentage_today, summary_pos_x + 1020, summary_pos_y + 20);
}
