/*-------------------------------------------------------------------------
 *                  UNIFAL - Universidade Federal de Alfenas.
 *                    BACHARELADO EM CIENCIA DA COMPUTACAO.
 * Trabalho..: Contagem de feijoes
 * Professor.: Luiz Eduardo da Silva
 * Aluno.....: Nicole Lima
 * Data......: 20/05/2024
*-------------------------------------------------------------------------*/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#if defined(_WIN32) || defined(__WIN64__) || defined(__CYGWIN__)
#include "..\\utils\\imagelib.h"
#elif defined(__linux__) || defined(__APPLE__)
#include "../utils/imagelib.h" 
#endif

/** Função auxiliar `find` para a função @rotula
    @return: representante de um conjunto
*/
int find(int parent[], int i){
    while (parent[i] != i)
        i = parent[i];
    return i;
}

/** Função auxiliar `Union` para a função @rotula
    Realiza a união de dois conjuntos @param i e @param j
*/
void Union(int parent[], int i, int j){
    int x = find(parent, i);
    int y = find(parent, j);
    parent[y] = x;
}

// Código de rotulação de componentes conexos desenvolvido em aula
image rotula (image img) {
    image result = img_clone(img);
    int w = img->nc;
    int h = img->nr;

    int *in = img->px;
    int *out = result->px;

    int rotulo = 0;
    int parent[1000];

    // Preenchendo 
    for (int i = 0; i < 1000; i++)
        parent[i] = i;

    // Percurso, varrendo todas as linhas exceto a ultima linha e coluna e primeira linha e coluna (borda da imagem)
    for (int i = 1; i < h - 1; i++)
        // Largura
        for (int j = 1; j < w - 1; j++)
        // Verificando os vizinhos de cima e de baixo para cada ponto (i,j)
        {
            int esq = i * w + j - 1;
            int cima = (i - 1) * w + j;
            int atual = i * w + j;

            int r = in[cima];
            int t = in[esq];
            int p = in[atual];
            if (p==1){
                // Se os vizinhos não foram rotulados ainda
                if (r == 0 && t == 0) out[atual] = ++rotulo;
                if (r == 1 && t == 0) out[atual] = out[cima];
                if (r == 0 && t == 1) out[atual] = out[esq];
                if (r == 1 && t == 1) {
                    // Vizinho de cima com o da esquerda
                    if (out[cima] != out[esq]){
                        out[atual] = out[esq]; // Recebe o vizinho da esquerda quando equivalentes
                        // printf("Rotulos equivalentes: %d, %d \n", out[cima], out[esq]);
                        // Tratando os rótulos equivalentes
                        Union(parent,out[cima], out[esq]);
                    }            
                    else
                        out [atual] = out[esq];
                }
            }
        }
    // Operacao pontual percorrendo o out todinho pra preencher com a nova equivalencia
    for (int i = 0; i < w * h; i++)
    {
        out[i] = find(parent, out[i]);
    }
    
    int rotulos_unicos[1000] = {0};
    for (int i = 0; i < w * h; i++) {
        if (out[i] != 0) {
            rotulos_unicos[out[i]] = 1;
        }
    }

    int componentes_conexos = 0;
    for (int i = 0; i < 1000; i++) {
        if (rotulos_unicos[i] == 1) {
            componentes_conexos++;
        }
    }

    printf("#componentes= %.2d\n", componentes_conexos);
        
    result->ml = rotulo;
    // mostrando o parent
    // for(int i = 0; i < 1000; i++) printf("parent[%d]: %d\n", i, parent[i]);

    return result;
}

/* Código genérico para limiarização desenvolvido em aula (vídeoaula Transformação de Intensidade) 
    Alterado para retornar uma imagem no formato .PBM para a função @rotula já desenvolvida em aula
*/
image limiar(image In, image Out) {
    int nr = In->nr;
    int nc = In->nc;
    int ml = In->ml;

    double T[ml + 1];
    
    // Definindo a transformação com limiar fixo de 95
    for (int i = 0; i < ml + 1; i++)
        T[i] = i < 95 ? 1 : 0; // Alteração de 0 : ml para 1 : 0
    
    // Aplicando a transformação a cada pixel da imagem de entrada
    for (int i = 0; i < nr * nc; i++)
        Out->px[i] = (int)T[In->px[i]]; // Atribuindo o valor transformado ao pixel correspondente na imagem de saída
    return Out;
}

void msg(char *s)
{
    printf("\nContagem de feijoes <3 <3 <3");
    printf("\n-------------------------------");
    printf("\nUsage:  %s  image-name[.pgm]\n\n", s);
    printf("    image-name[.pgm] is image file in pgm format \n\n");
    exit(1);
}

image contaFeijao(image img) {
    image result;
    result = img_clone(img);

    // TODO: transformar a imagem de entrada em binaria (limiarização), separando o fundo das formas
    image binaria = img_create(img->nr, img->nc, 1, 1);
    binaria = limiar(img, binaria);
    img_put(binaria, "binaria.pbm",BW);

    // // TODO: rotular a imagem binária 
    image rotulada = rotula(binaria);

    // TODO: calcular imagem distância 

    // TODO: contar os feijoes
    // int feijoes = 0;
    // printf("#componentes= %.2i", feijoes);

    return rotulada;
}

/*-------------------------------------------------------------------------
 * main function
 *-------------------------------------------------------------------------*/
int main(int argc, char *argv[]) {
    int nc, nr, ml, tp;
    char *p, nameIn[100], nameOut[100], cmd[110];
    image In, Out;

    if (argc < 2)
        msg(argv[0]);

    //-- define input/output file name
    img_name(argv[1], nameIn, nameOut, GRAY, GRAY);

    //-- read image
    In = img_get(nameIn, GRAY);

    // //-- initialize output image
    Out = img_clone(In);

    //-- transformation 
    Out = contaFeijao(In);

    //-- save image
    img_put(Out, nameOut, GRAY);

    //-- show image
    // sprintf(cmd, "%s %s &", VIEW, nameOut);
    // puts(cmd);

    //-- free memory
    img_free(In);
    img_free(Out);

    return 0;
}

