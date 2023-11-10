

///    A atividade consistem em desenvolver uma aplicação em linguagem C que armazene dados dos alunos em memória 
///    e depois em um arquivo binário e que atenda os requisitos abaixo. Faça um comentário no início do código
///    explicando o que você fez e também em partes do código que seja necessário para o entendimento.
///    1) Armazenamento das dados em memória de forma dinâmica.
///    2) Gravar e Carregar os dados de um arquivo binário.
///    3) Deve conter um menu de operações para manipular os dados.
///    4) Informações que devem ser armazenadas: Nome, RA, data de nascimento, 2 endereços
///    (rua, número, bairro, cidade e estado), nomes e médias de 5 disciplinas.
///    5) A estrutura da aplicação disponibilizada no Github no endereço
///    https://github.com/edersonschmeing/algoritmos-e-logica-de-programacao-em-c/tree/main/
///    2023-2/12-trabalho-2


 //// incluindo libraries 
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>  
#include <string.h>  



/// definindo estrutura de aluno

#define MAX_ALUNOS 20 // constante de maximo de alunos
#define MAX_LENGTH 50 // constante de tamanho maximo de strings 
#define NUM_DISCIPLINA 5  // constante do numero de disciplinas
#define NUM_ENDERECO 2 // constatnte do numero maximo de enderecos

typedef struct {   /// definindo uma estrutura sem alias
    char rua[MAX_LENGTH];   /// definicoes das variaveis
    int numero;
    char bairro[MAX_LENGTH];
    char cidade[MAX_LENGTH];
    char estado[MAX_LENGTH];
} Endereco;

typedef struct {
    char nome[MAX_LENGTH];
    int ra;
    char nascimento[MAX_LENGTH];
    Endereco endereco[NUM_ENDERECO];
    char disciplina_nome[NUM_DISCIPLINA][MAX_LENGTH]; /// insetindo a estrutura de endereco dentro da estrutura de alunos, com o maximo de 2 estruturas
    float disciplina_media[NUM_DISCIPLINA];
} Aluno;

struct array_dinamico {
    bool ordenado;  // true indica que quer manter o array ordenado. 
    int tamanho;    // número máximo de elementos que podemos armazenados no array.
    int quantidade; // quantidade atual de elementos armazenados
    Aluno **ptr_dados;     // array de int que representa nossos dados. 
};
typedef struct array_dinamico Array_Dinamico;

Array_Dinamico* criar_array_dinamico(int tamanho_array, bool ordenado) {

    Array_Dinamico *array_dinamico = (Array_Dinamico*) malloc(sizeof(Array_Dinamico));
    array_dinamico->ordenado = ordenado;
    array_dinamico->quantidade = 0;
    array_dinamico->tamanho = tamanho_array;
    array_dinamico->ptr_dados = (Aluno**) calloc(tamanho_array, sizeof(Aluno*));

    return array_dinamico;

}

Aluno* criar_aluno() {
    Aluno* novo_aluno = (Aluno*)malloc(sizeof(Aluno));
    if (novo_aluno != NULL) {
        return novo_aluno;
    } else {
        printf("Erro ao alocar memória para o novo aluno.\n");
        return NULL;
    }
}

int comparar_por_nome(const void *a, const void *b) {
    const Aluno *alunoA = *(const Aluno **)a;
    const Aluno *alunoB = *(const Aluno **)b;

    return strcmp(alunoA->nome, alunoB->nome);
}

void ordenar_por_nome(Array_Dinamico *array_dinamico) {
    qsort(array_dinamico->ptr_dados, array_dinamico->quantidade, sizeof(Aluno*), comparar_por_nome);
}

void ordenar_por_ra(Array_Dinamico *array_dinamico) {
    int n = array_dinamico->quantidade;
    int i, j;
    for (i = 0; i < n - 1; i++) {
        for (j = 0; j < n - i - 1; j++) {
            if ((*array_dinamico->ptr_dados[j]).ra > (*array_dinamico->ptr_dados[j + 1]).ra) {
                // Troca os alunos de posição se o RA for maior
                Aluno *temp = array_dinamico->ptr_dados[j];
                array_dinamico->ptr_dados[j] = array_dinamico->ptr_dados[j + 1];
                array_dinamico->ptr_dados[j + 1] = temp;
            }
        }
    }
}

void destruir_array_dinamico(Array_Dinamico **array_dinamico_endereco_de_memoria) {

    Array_Dinamico *array_dinamico = *array_dinamico_endereco_de_memoria;
    
    for (int i = 0; i < array_dinamico->quantidade; i++) {
        free(array_dinamico->ptr_dados[0]);
        array_dinamico->ptr_dados[0] = NULL;
    }
    free(array_dinamico->ptr_dados);
    free(array_dinamico);    
    *array_dinamico_endereco_de_memoria = NULL;

}
bool esta_cheio_array_dinamico(Array_Dinamico* array_dinamico) {
    if (array_dinamico->quantidade >= array_dinamico->tamanho) { /// verifica se quantidade de alunos é maior que o tamanho do array e retorna resultado
        return 1;
    }else{
        return 0;
    }
}

void limpa_terminal(){
    #ifdef _WIN32
    system("cls");  // Se for Windows
    #else
    system("clear");  // Se for Unix/Linux
    #endif
}

int busca_sequencial_array_dinamico(Array_Dinamico *array_dinamico, int valor) {  // busca indice do array que corresponde ao RA
    for (int i = 0; i < array_dinamico->quantidade; i++) {
        Aluno *aluno = array_dinamico->ptr_dados[i];
        if (aluno->ra == valor)
            return i; //retorna o index do elemento.
    }    
    return -1; //caso não encontre o elemente, retorna -1 com index.
}

void imprimir_aluno(Array_Dinamico *array_dinamico, int index){
    Aluno *aluno = array_dinamico->ptr_dados[index];
    printf("Aluno %d:\n", index + 1);
    printf("Nome: %s\n", aluno->nome);
    printf("RA: %d\n", aluno->ra);
    printf("Data de Nascimento: %s\n", aluno->nascimento);
    for (int j = 0; j < NUM_ENDERECO; j++) {
        printf("Endereco %d:\n", j + 1);
        printf("Rua: %s\n", aluno->endereco[j].rua);
        printf("Número: %d\n", aluno->endereco[j].numero);
        printf("Bairro: %s\n", aluno->endereco[j].bairro);
        printf("Cidade: %s\n", aluno->endereco[j].cidade);
        printf("Estado: %s\n", aluno->endereco[j].estado);
    }
    for (int j = 0; j < NUM_DISCIPLINA; j++) {
        printf("Disciplina %d:\n", j + 1);
        printf("Nome: %s\n", aluno->disciplina_nome[j]);
        printf("Média: %.2f\n", aluno->disciplina_media[j]);
    }
    printf("------------------------\n");
}

int quantidade_array_dinamico(Array_Dinamico *array_dinamico) {
    if(array_dinamico->quantidade){
        printf("Estao cadastrados %d aluno(s)!\n", array_dinamico->quantidade);
    }else{
        printf("Erro ao obter alunos ou nao ha nenhum aluno cadastrado!\n");
    }
    return 0;
}

int tamanho_array_dinamico(Array_Dinamico *array_dinamico) {
    if(array_dinamico->tamanho){
        printf("O array tem %d espaços alocados!\n", array_dinamico->tamanho);
    }else{
        printf("Erro ao obter tamanho do array!\n");
    }
    return 0;
}

void organizar_array_dinamico(Array_Dinamico *array_dinamico, int posicao_removida) {
    if (array_dinamico->quantidade <= 0 || posicao_removida < 0 || posicao_removida >= array_dinamico->quantidade) {
            printf("Posição inválida ou array vazio.\n");
            return;
    }

        // Movendo os elementos seguintes uma posição para tras no array
    for (int i = posicao_removida; i < array_dinamico->quantidade; i++) {
        array_dinamico->ptr_dados[i] = array_dinamico->ptr_dados[i + 1];
    }
    // Reduzindo a quantidade de alunos
    array_dinamico->quantidade--;

    printf("Array dinamico reorganizado!\n");
}

void aumentar_array_dinamico(Array_Dinamico *array_dinamico) { // dobra o tamanho do array
    
    printf("Aumentando 2 vezes o tamanho atual do array( 2 * %d )\n", array_dinamico->tamanho);

    Aluno **temp = array_dinamico->ptr_dados; /// Armazena dados dos alunos temporariamente

    array_dinamico->tamanho = array_dinamico->tamanho * 2; /// dobra o tamanho do array

    array_dinamico->ptr_dados = (Aluno**) calloc(array_dinamico->tamanho, sizeof(Aluno*));  /// realoca espaco na memoria

    for (int i = 0; i < array_dinamico->tamanho; i++ ) { 
       array_dinamico->ptr_dados[i] = temp[i]; // copia dados temporarios para o array dinamico redimensionado
    }
    free(temp);  // limpa a variavel temporaria

}

void diminuir_array_dinamico(Array_Dinamico *array_dinamico) { //diminuir o tamanho do array pela metade quando tiver 1/4 ou 25% cheio   

    if ((array_dinamico->quantidade < array_dinamico->tamanho / 4) && 
        (array_dinamico->tamanho >= 4)) { 
       
        printf("Diminuindo o tamanho atual do array pela metade ( %d / 2 )\n", array_dinamico->tamanho); 
 
        Aluno **temp = array_dinamico->ptr_dados; 
        array_dinamico->tamanho /= 2;
        
        array_dinamico->ptr_dados = (Aluno**)calloc(array_dinamico->tamanho, sizeof(Aluno*)); 

        for (int j = 0; j < array_dinamico->tamanho; j++ ) { 
            array_dinamico->ptr_dados[j] = temp[j];
        }
        free(temp);
    }
}

void remover_aluno(Array_Dinamico *array_dinamico, int index){
    
    if (index != -1) { // Verifica se o aluno com o RA fornecido foi encontrado.
        Aluno *aluno_a_ser_removido = array_dinamico->ptr_dados[index];

        // Libera a memória alocada para esse aluno
        printf("%p \n%d\n", aluno_a_ser_removido, aluno_a_ser_removido->ra);
        free(aluno_a_ser_removido);
        aluno_a_ser_removido = NULL;
        organizar_array_dinamico(array_dinamico, index);

        diminuir_array_dinamico(array_dinamico);

        printf("Aluno removido com sucesso.\n");
    } else {
        printf("Aluno não encontrado.\n");
    }
}

void adicionar_aluno(Array_Dinamico *array_dinamico, Aluno *novo_aluno) {
    if (array_dinamico->quantidade < array_dinamico->tamanho) {  //// VERIFICA SE QUANTIDADE DE ALUNOS É MENOR QUE O TAMANHO DO ARRAY
        array_dinamico->ptr_dados[array_dinamico->quantidade] = novo_aluno;
        array_dinamico->quantidade++;
        printf("Aluno adicionado com sucesso!\n");
    } else {
        printf("O array dinâmico está cheio.\n");
        aumentar_array_dinamico(array_dinamico); /// REDIMENSIONA O ARRAY DINAMICO PARA PODER SALVAR MAIS ALUNOS
        adicionar_aluno(array_dinamico, novo_aluno); /// UTILIZANDO FUNCAO RECURSIVA PARA CHAMAR NOVAMENTE A FUNCTION E CONFIRMAR O REDIMENSIONAMENTO E A ADICAO
    }
}

void carregar_alunos(Array_Dinamico *array_dinamico){
    FILE *arquivo = fopen("alunos.bin", "rb");  /// carrega arquivo binario e utiliza a funcao RB para le-lo

    if (arquivo == NULL) { // verifica se arquivo existe
        printf("Erro ao abrir o arquivo para carregar.\n");
        return;
    }

    int quantidade;
    fread(&quantidade, sizeof(int), 1, arquivo); /// carrega a quantidade de alunos no arquivo para usar no laco

    for (int i = 0; i < quantidade; i++) { 
        Aluno *novo_aluno = criar_aluno(); // cria um novo aluno no vetor
        fread(novo_aluno, sizeof(Aluno), 1, arquivo); // passa os dados do aluno para o vetor
        adicionar_aluno(array_dinamico, novo_aluno); // adiciona o aluno no array dinamico
    }

    fclose(arquivo); // fecha o arquivo

    printf("Dados dos alunos foram carregados do arquivo binário.\n"); /// SUCESSO

}

void salvar_alunos(Array_Dinamico *array_dinamico) {
    FILE *arquivo = fopen("alunos.bin", "w");  /// passa o nome do arquivo e utiliza W para escrever um novo, se ja existir destroi o conteudo anterior

    if (arquivo == NULL) { /// verifica se arquivo existe (NAO É NECESSARIO POIS UTILIZAMOS 'W' CASO NAO EXISTIR O ARQUIVO É CRIADO)
        printf("Erro ao abrir o arquivo para salvar.\n");
        return;
    }

    fwrite(&(array_dinamico->quantidade), sizeof(int), 1, arquivo); // escreve a quantidade no arquivo

    for (int i = 0; i < array_dinamico->quantidade; i++) {
        fwrite(array_dinamico->ptr_dados[i], sizeof(Aluno), 1, arquivo); // escreve dados dos alunos no arquivo
    }

    fclose(arquivo); // fecha

    printf("Dados dos alunos foram salvos no arquivo binário.\n"); // sucesso
    
}

void preencher_detalhes_aluno(Array_Dinamico *array_dinamico) { /// funcao para colher dados
    Aluno *novo_aluno = criar_aluno();

    printf("Nome do aluno: ");
    scanf(" %[^\n]", novo_aluno->nome);
    printf("RA do aluno: ");
    scanf("%d", &novo_aluno->ra);
    printf("Data de Nascimento do aluno: ");
    scanf(" %[^\n]", novo_aluno->nascimento);

    for (int i = 0; i < NUM_ENDERECO; i++) {
        printf("Endereco %d:\n", i + 1);
        printf("Rua: ");
        scanf(" %[^\n]", novo_aluno->endereco[i].rua);
        printf("Número: ");
        scanf("%d", &novo_aluno->endereco[i].numero);
        printf("Bairro: ");
        scanf(" %[^\n]", novo_aluno->endereco[i].bairro);
        printf("Cidade: ");
        scanf(" %[^\n]", novo_aluno->endereco[i].cidade);
        printf("Estado: ");
        scanf(" %[^\n]", novo_aluno->endereco[i].estado);
    }

    for (int i = 0; i < NUM_DISCIPLINA; i++) {
        printf("Disciplina %d:\n", i + 1);
        printf("Nome: ");
        scanf(" %[^\n]", novo_aluno->disciplina_nome[i]);
        printf("Média: ");
        scanf("%f", &novo_aluno->disciplina_media[i]);
    }
    adicionar_aluno(array_dinamico, novo_aluno);
}

void listar_alunos(Array_Dinamico *array_dinamico) {
    for (int i = 0; i < array_dinamico->quantidade; i++) {
        imprimir_aluno(array_dinamico, i);
    }
}

void creditos(){
    printf("\033[1;34m\n\n=========  Trabalho desenvolvido por: ========== \n");
    printf("|| Simon Schroder Lopes        ||  RA: 233257 ||\n");
    printf("|| Crystian Mazali Piekarczyk  ||  RA: 231422 ||\n");
    printf("================================================\n\033[1;0m");
}

int menu() { 
    
    /// variaveis podem ser declaradas no main e passadas por paramentro para o menu
    int tamanho = 4;
    bool ordenado = false;
    Array_Dinamico *array_dinamico = criar_array_dinamico(tamanho, ordenado); /// inicializa o array dinamico

    char opcao;
    int RA;

    while (1) { /// while para manter menu ativo mesmo apos retorno da funcao escolhida
        printf("\nMenu:\n");
        printf("1. Adicionar Aluno\n");
        printf("2. Listar Alunos\n");
        printf("3. Buscar Aluno\n");
        printf("4. Remover Aluno\n");
        printf("5. Verificar se aluno existe\n");
        printf("6. Quantidade de alunos\n");
        printf("7. Tamanho do array\n");
        printf("8. Ordenar alunos por RA\n");
        printf("A. Ordenar alunos por Nome\n");
        printf("B. Salvar Alunos em Arquivo\n");
        printf("C. Carregar Alunos de Arquivo\n");
        printf("9. Limpar terminal\n");
        printf("0. Sair\n");
        printf("Escolha uma opção: ");
        scanf("%c", &opcao);
        printf("\n");

        switch (opcao) {
            case '1':
                preencher_detalhes_aluno(array_dinamico);
                break;
            case '2':
                listar_alunos(array_dinamico);
                break;
            case '3':
                printf("Digite o RA a buscar: ");
                scanf("%d", &RA);
                int index_busca = busca_sequencial_array_dinamico(array_dinamico, RA);

                if (index_busca != -1)
                {
                    imprimir_aluno(array_dinamico, index_busca);
                } else
                {
                    printf("Aluno não encontrado!\n");
                }
                
                break;
            case '4':
                printf("Digite o RA a ser removido: ");
                scanf("%d", &RA);
                int index_remove = busca_sequencial_array_dinamico(array_dinamico, RA);
                remover_aluno(array_dinamico, index_remove);
                break;
            case '5':
                printf("Digite o RA a pesquisar: ");
                scanf("%d", &RA);
                int index_pesquisa = busca_sequencial_array_dinamico(array_dinamico, RA);

                if (index_busca != -1)
                {
                    printf("Aluno cadastrado como Aluno %d\n", index_pesquisa + 1);
                } else
                {
                    printf("Aluno não encontrado!\n");
                }
                break;
            case '6':
                quantidade_array_dinamico(array_dinamico);
                break;
            case '7':
                tamanho_array_dinamico(array_dinamico);
                break;
            case '8':
                ordenar_por_ra(array_dinamico);
                break;
            case 'A' | 'a':
                ordenar_por_nome(array_dinamico);
                break;
            case 'B' | 'b':
                salvar_alunos(array_dinamico);
                break;
            case 'C' | 'c':
                carregar_alunos(array_dinamico);
                break;
            case '9':
                limpa_terminal();
                break;
            case '0':
                creditos();
                // Libere a memória alocada e saia do programa
                destruir_array_dinamico(&array_dinamico);
                return 0;
            default:
                printf("Opcao invalida.\n");
        }
     while (getchar() != '\n');
    }

}

int main() {
    creditos();
    menu();

}