#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <stdio.h>

const char *keywords_c[] = {
  "auto", "double", "int", "struct", "break", "else", "long", "switch",
  "case", "enum", "register", "typedef", "char", "extern", "return", "union",
  "const", "float", "short", "unsigned", "continue", "for", "signed", "void",
  "default", "goto", "sizeof", "volatile", "do", "if", "static", "while",
  NULL,
};

const char *keywords_za[] = {
  "auto", "dubble", "nom", "strukt", "breek", "anders", "lang", "?",
  "geval", "?", "register", "tipedef", "kara", "buiten", "teruggee", "unie",
  "knost", "?", "kort", "?", "gaanaan", "vir", "?", "?",
  "?", "gaanna", "grootevan", "?", "doen", "as", "staties", "terwyl",
  NULL,
};

const char *keywords_es[] = {
  "auto", "doble", "entero", "estructura", "salir", "sino", "grande", "opcion",
  "caso", "lista", "registro", "tipo", "caracter", "externo", "retornar", "union",
  "fijo", "flotante", "corto", "sinsigno", "continuar", "para", "signo", "vacio",
  "otros", "saltar", "longitud", "volatil", "hacer", "si", "estatico", "mientras",
  NULL,
};

const char *keywords_tok[] = {
  "palijan", "tu", "pini", "kulupu", "weka", "laala", "suli", "anu",
  "ijo", "mute", "lupa", "waleja", "kalama", "nasa", "pana", "wan",
  "kiwen", "piniala", "lili", "unsigned?", "awen", "aleweka", "signed?", "ala",
  "ante", "tawa", "suli", "awenala", "pali", "la", "tawaala", "tenpo",
  NULL,
};

const char *keywords_yay[] = {
  "auto", "double", "math", "party", "cancel", "but", "extreme", "choose",
  "please", "homework", "drawer", "invent", "letter", "popular", "lend", "marriage",
  "therock", "real", "mydick", "unloyal", "pass", "five", "loyal", "crisis",
  "nah", "fly", "measure", "explosive", "do", "if", "ghosted", "whilst",
  NULL,
};

int isname(char chr) {
  return isalnum(chr) || chr == '_' || chr == '$';
}

int agnc_find(const char **in_words, const char *word) {
  int index = 0;
  
  while (in_words[index]) {
    if (!strcmp(in_words[index], word)) return index;
    index++;
  }
  
  return -1;
}

void agnc_run(FILE *in_file, const char **in_words, FILE *out_file, const char **out_words) {
  char buffer[256];
  int length = 0;
  
  int in_string = 0;
  
  while (!feof(in_file)) {
    char chr = fgetc(in_file);
    int set_string = 0;
    
    if (chr == '\\' && in_string) {
      fputc(chr, out_file);
      chr = fgetc(in_file);
      fputc(chr, out_file);
      
      continue;
    }
    
    if (chr == '"' || chr == '\'') {
      if (in_string == chr) {
        in_string = 0;
      } else if (!in_string) {
        set_string = 1;
      }
    }
    
    if (in_string) {
      fputc(chr, out_file);
    } else {
      if (isname(chr)) {
        buffer[length++] = chr;
      } else {
        if (length > 0) {
          buffer[length] = '\0';
          int index = agnc_find(in_words, buffer);
          
          if (index >= 0) {
            fprintf(out_file, "%s", out_words[index]);
          } else {
            fprintf(out_file, "%s", buffer);
          }
          
          length = 0;
        }
        
        fputc(chr, out_file);
      }
    }
    
    if (set_string) {
      in_string = chr;
    }
  }
  
  if (length > 0) {
    buffer[length] = '\0';
    int index = agnc_find(in_words, buffer);
    
    if (index >= 0) {
      fprintf(out_file, "%s", out_words[index]);
    } else {
      fprintf(out_file, "%s", buffer);
    }
  }
}

void agnc_to(const char *path, const char *lang) {
  FILE *in_file = fopen(path, "r");
  if (!in_file) exit(1);
  
  char new_path[256];
  
  const char *last = strrchr(path, '.');
  if (!last) last = path + strlen(path);
  
  memcpy(new_path, path, last - path);
  sprintf(new_path + (last - path), ".%s.c", lang);
  
  const char **out_words = NULL;
  
  if (!strcmp(lang, "za")) out_words = keywords_za;
  else if (!strcmp(lang, "es")) out_words = keywords_es;
  else if (!strcmp(lang, "tok")) out_words = keywords_tok;
  else if (!strcmp(lang, "yay")) out_words = keywords_yay;
  else exit(1);
  
  FILE *out_file = fopen(new_path, "w");
  if (!out_file) exit(1);
  
  agnc_run(in_file, keywords_c, out_file, out_words);
  
  fclose(out_file);
  fclose(in_file);
}

void agnc_from(const char *path, const char *lang) {
  FILE *in_file = fopen(path, "r");
  if (!in_file) exit(1);
  
  char new_path[256], buffer[16];
  sprintf(buffer, ".%s.c", lang);
  
  const char *last = strstr(path, buffer);
  if (!last) last = path + strlen(path);
  
  memcpy(new_path, path, last - path);
  strcpy(new_path + (last - path), ".c");
  
  const char **in_words = NULL;
  
  if (!strcmp(lang, "za")) in_words = keywords_za;
  else if (!strcmp(lang, "es")) in_words = keywords_es;
  else if (!strcmp(lang, "tok")) in_words = keywords_tok;
  else if (!strcmp(lang, "yay")) in_words = keywords_yay;
  else exit(1);
  
  FILE *out_file = fopen(new_path, "w");
  if (!out_file) exit(1);
  
  agnc_run(in_file, in_words, out_file, keywords_c);
  
  fclose(out_file);
  fclose(in_file);
}

int main(int argc, const char **argv) {
  if (argc < 2) return 1;
  
  if (!strncmp(argv[1], "to_", 3)) {
    for (int i = 2; i < argc; i++) {
      agnc_to(argv[i], argv[1] + 3);
    }
  } else if (!strncmp(argv[1], "from_", 5)) {
    for (int i = 2; i < argc; i++) {
      agnc_from(argv[i], argv[1] + 5);
    }
  } else {
    return 1;
  }
  
  return 0;
}
