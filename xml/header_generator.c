#include "header_generator.h"
#include "utils.h"

/******************************************************************************/
/*                                Defines                                     */
/******************************************************************************/
#define HEADER_HEADER "#ifndef I2C_INTERFACE_ADDR_H\n#define I2C_INTERFACE_ADDR_H\n\n"
#define HEADER_FOOTER "\n#endif /* I2C_INTERFACE_ADDR_H */\n"

typedef enum modifier_s {
    NORMAL,
    LOW,
    HIGH
} modifier_t;

/******************************************************************************/
/*                           Static functions                                 */
/******************************************************************************/
static void write_entry_content(FILE *file, interface_element_t *element, modifier_t modifier) {
    int index = 0;
    int address;
    fprintf(file, "#define ");

    while ((element->name[index] != ' ') && (element->name[index] != '\0')) {
        if(is_small(element->name[index])) {
            fputc(element->name[index] + 'A' - 'a', file);
        } else {
            fputc(element->name[index], file);
        }
        index++;
    }

    switch (modifier) {
        case NORMAL:
            address = element->address;
            break;
        case LOW:
            address = element->address;
            fprintf(file, "_LOW");
            break;
        case HIGH:
            address = element->address + 2;
            fprintf(file, "_HIGH");
            break;
    }
    fprintf(file, "_ADDR 0x%02xU\n", address);
}

static void write_entry(FILE *file, interface_element_t *element) {
    /* Check input parameters */
    if (file == NULL) {
        return;
    } else if (element == NULL) {
        return;
    }

    if (element->size < 32) {
        write_entry_content(file, element, NORMAL);
    } else {
        write_entry_content(file, element, LOW);
        write_entry_content(file, element, HIGH);
    }

}

/******************************************************************************/
/*                           Public functions                                 */
/******************************************************************************/
FILE* init_header_file(char *file_name) {
    FILE *file;
    int ret;

    /* Check input parameters */
    if (file_name == NULL) {
        return NULL;
    }

    file = fopen(file_name, "w");
    if (file == NULL) {
        return NULL;
    }

    ret = fprintf(file, HEADER_HEADER);
    if (ret < 0) {
        printf("Error when writing header in %s", file_name);
    }

    return file;
}

int write_header_file(FILE *file, interface_element_t *entry) {
    /* Check input parameters */
    if (file == NULL) {
        return -1;
    } else if (entry == NULL) {
        return -2;
    }

    while (entry != NULL) {
        write_entry(file, entry);
        entry = entry->next;
    }

    return 0;
}

void close_header_file(FILE *file) {
    /* Check input parameters */
    if (file == NULL) {
        return;
    }

    fprintf(file, HEADER_FOOTER);
    fclose(file);
}
