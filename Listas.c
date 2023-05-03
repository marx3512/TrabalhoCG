#define TOLERANCIA 10

typedef struct ponto {
    int x;
    int y;
    int minimoX;
    int maximoX;
    int minimoY;
    int maximoY;
    int esquerda;
    int direita;
    int aCima;
    int aBaixo;
} Ponto;

typedef struct reta {
    Ponto p1;
    Ponto p2;
} Reta;

typedef struct no {
    Reta reta;
    struct no* proximo;
} No;

typedef struct no_ponto {
    Ponto ponto;
    struct no_ponto* proximo;
} NoPonto;

typedef struct lista_ponto {
    NoPonto* inicio;
    int tamanho;
} ListaPonto;

typedef struct lista_reta {
    No* inicio;
    int tamanho;
} Lista_reta;

Lista_reta* criar_lista_reta() {
    Lista_reta* lista = (Lista_reta*) malloc(sizeof(Lista_reta));
    lista->inicio = NULL;
    lista->tamanho = 0;
    return lista;
}

ListaPonto* criar_lista_ponto() {
    ListaPonto* lista = (ListaPonto*) malloc(sizeof(ListaPonto));
    lista->inicio = NULL;
    lista->tamanho = 0;
    return lista;
}

void inserir_elemento_reta(Lista_reta* lista, Reta reta) {
    No* novo_no = (No*) malloc(sizeof(No));
    novo_no->reta = reta;
    novo_no->proximo = lista->inicio;
    lista->inicio = novo_no;
    lista->tamanho++;
}

void inserir_elemento_ponto(ListaPonto* lista, Ponto ponto) {
    NoPonto* novo_no = (NoPonto*) malloc(sizeof(NoPonto));
    novo_no->ponto = ponto;
    novo_no->proximo = lista->inicio;
    lista->inicio = novo_no;
    lista->tamanho++;
}

void remover_elemento_reta(Lista_reta* lista, Reta reta) {
    No* no_atual = lista->inicio;
    No* no_anterior = NULL;
    while (no_atual != NULL && (no_atual->reta.p1.x != reta.p1.x || no_atual->reta.p1.y != reta.p1.y || no_atual->reta.p2.x != reta.p2.x || no_atual->reta.p2.y != reta.p2.y)) {
        no_anterior = no_atual;
        no_atual = no_atual->proximo;
    }
    if (no_atual != NULL) {
        if (no_anterior == NULL) {
            lista->inicio = no_atual->proximo;
        } else {
            no_anterior->proximo = no_atual->proximo;
        }
        free(no_atual);
        lista->tamanho--;
    }
}

void remover_elemento_ponto(ListaPonto* lista, Ponto ponto) {
    NoPonto* no_atual = lista->inicio;
    NoPonto* no_anterior = NULL;
    while (no_atual != NULL && (no_atual->ponto.x != ponto.x || no_atual->ponto.y != ponto.y)) {
        no_anterior = no_atual;
        no_atual = no_atual->proximo;
    }
    if (no_atual != NULL) {
        if (no_anterior == NULL) {
            lista->inicio = no_atual->proximo;
        } else {
            no_anterior->proximo = no_atual->proximo;
        }
        free(no_atual);
        lista->tamanho--;
    }
}

Ponto acessar_indice_ponto(ListaPonto* lista, int indice){
    int count = 0;
    NoPonto* atual = lista->inicio;
    while (atual != NULL) {
        printf("asfasf");
        if(count == indice) {
            printf("Deu certo\n");
            return atual->ponto;
        }
        count++;
        atual = atual->proximo;
    }
    printf("indice fora dos limites da lista.\n");
    //return NULL;
}

Ponto acessar_hitbox_ponto(ListaPonto* lista, int mouseX, int mouseY){
    NoPonto* atual = lista->inicio;
    while (atual != NULL){
        if(mouseX <= atual->ponto.maximoX && mouseX >= atual->ponto.minimoX){
            if(mouseY <= atual->ponto.maximoY && mouseY >= atual->ponto.minimoY){
                return atual->ponto;
            }
        }
        atual = atual->proximo;
    }
    Ponto pontoVazio = {0, 0, 0, 0, 0, 0};
    return pontoVazio;
}
//Funcao para selecionar retas
void checarPontosRetas(Lista_reta* lista, int mouseX, int mouseY){
    No* atual = lista->inicio;
    while (atual != NULL){
        //Cima esquerda
        if(atual->reta.p1.x <= mouseX - TOLERANCIA && atual->reta.p1.y >= (700 - mouseY) + TOLERANCIA){
            atual->reta.p1.aBaixo = 0;
            atual->reta.p1.aCima= 1;
            atual->reta.p1.direita = 0;
            atual->reta.p1.esquerda = 1;
            printf("Quadrante cima esquerda p1, codigo %d%d%d%d\n", atual->reta.p1.aBaixo,atual->reta.p1.aCima,atual->reta.p1.direita,atual->reta.p1.esquerda);

        }
        if(atual->reta.p2.x <= mouseX - TOLERANCIA && atual->reta.p2.y >= (700 - mouseY) + TOLERANCIA){
            atual->reta.p2.aBaixo = 0;
            atual->reta.p2.aCima= 1;
            atual->reta.p2.direita = 0;
            atual->reta.p2.esquerda = 1;
            printf("Quadrante cima esquerda p2, codigo %d%d%d%d\n", atual->reta.p2.aBaixo,atual->reta.p2.aCima,atual->reta.p2.direita,atual->reta.p2.esquerda);
        }
        //Cima
        if(atual->reta.p1.x >= mouseX - TOLERANCIA && atual->reta.p1.x <= mouseX + TOLERANCIA){
            if(atual->reta.p1.y >= (700 - mouseY) + TOLERANCIA){
                atual->reta.p1.aBaixo = 0;
                atual->reta.p1.aCima= 1;
                atual->reta.p1.direita = 0;
                atual->reta.p1.esquerda = 0;
                printf("Quadrante cima p1, codigo %d%d%d%d\n", atual->reta.p1.aBaixo,atual->reta.p1.aCima,atual->reta.p1.direita,atual->reta.p1.esquerda);
            }
        }
        if(atual->reta.p2.x >= mouseX - TOLERANCIA && atual->reta.p2.x <= mouseX + TOLERANCIA){
            if(atual->reta.p2.y >= (700 - mouseY) + TOLERANCIA){
                atual->reta.p2.aBaixo = 0;
                atual->reta.p2.aCima= 1;
                atual->reta.p2.direita = 0;
                atual->reta.p2.esquerda = 0;
                printf("Quadrante cima p2, codigo %d%d%d%d\n", atual->reta.p2.aBaixo,atual->reta.p2.aCima,atual->reta.p2.direita,atual->reta.p2.esquerda);
            }
        }
        //Cima direita
        if(atual->reta.p1.x >= mouseX + TOLERANCIA && atual->reta.p1.y >= (700 - mouseY) + TOLERANCIA ){
            atual->reta.p1.aBaixo = 0;
            atual->reta.p1.aCima= 1;
            atual->reta.p1.direita = 1;
            atual->reta.p1.esquerda = 0;
            printf("Quadrante cima direita p1, codigo %d%d%d%d\n", atual->reta.p1.aBaixo,atual->reta.p1.aCima,atual->reta.p1.direita,atual->reta.p1.esquerda);
        }
        if(atual->reta.p2.x >= mouseX + TOLERANCIA && atual->reta.p2.y >= (700 - mouseY) + TOLERANCIA ){
            atual->reta.p2.aBaixo = 0;
            atual->reta.p2.aCima= 1;
            atual->reta.p2.direita = 1;
            atual->reta.p2.esquerda = 0;
            printf("Quadrante cima direita p2, codigo %d%d%d%d\n", atual->reta.p2.aBaixo,atual->reta.p2.aCima,atual->reta.p2.direita,atual->reta.p2.esquerda);
        }
        //Esquerda
        if(atual->reta.p1.x <= mouseX - TOLERANCIA){
            if(atual->reta.p1.y >= (700 - mouseY) - TOLERANCIA && atual->reta.p1.y <= (700 - mouseY) + TOLERANCIA){
                atual->reta.p1.aBaixo = 0;
                atual->reta.p1.aCima= 0;
                atual->reta.p1.direita = 0;
                atual->reta.p1.esquerda = 1;
                printf("Quadrante esquerda p1, codigo %d%d%d%d\n", atual->reta.p1.aBaixo,atual->reta.p1.aCima,atual->reta.p1.direita,atual->reta.p1.esquerda);
            }
        }
        if(atual->reta.p2.x <= mouseX - TOLERANCIA){
            if(atual->reta.p1.y >= (700 - mouseY) - TOLERANCIA && atual->reta.p1.y <= (700 - mouseY) + TOLERANCIA){
                atual->reta.p2.aBaixo = 0;
                atual->reta.p2.aCima= 0;
                atual->reta.p2.direita = 0;
                atual->reta.p2.esquerda = 1;
                printf("Quadrante esquerda p2, codigo %d%d%d%d\n", atual->reta.p2.aBaixo,atual->reta.p2.aCima,atual->reta.p2.direita,atual->reta.p2.esquerda);
            }
        }
        //Centro
        if(atual->reta.p1.x >= mouseX - TOLERANCIA && atual->reta.p1.x <= mouseX + TOLERANCIA){
            if(atual->reta.p1.y >= (700 - mouseY) - TOLERANCIA && atual->reta.p1.y <= (700 - mouseY) + TOLERANCIA){
                atual->reta.p1.aBaixo = 0;
                atual->reta.p1.aCima= 0;
                atual->reta.p1.direita = 0;
                atual->reta.p1.esquerda = 0;
                printf("Quadrante centro p1, codigo %d%d%d%d\n", atual->reta.p1.aBaixo,atual->reta.p1.aCima,atual->reta.p1.direita,atual->reta.p1.esquerda);
            }
        }
        if(atual->reta.p2.x >= mouseX - TOLERANCIA && atual->reta.p2.x <= mouseX + TOLERANCIA){
            if(atual->reta.p2.y >= (700 - mouseY) - TOLERANCIA && atual->reta.p2.y <= (700 - mouseY) + TOLERANCIA){
                atual->reta.p2.aBaixo = 0;
                atual->reta.p2.aCima= 0;
                atual->reta.p2.direita = 0;
                atual->reta.p2.esquerda = 0;
                printf("Quadrante centro p2, codigo %d%d%d%d\n", atual->reta.p2.aBaixo,atual->reta.p2.aCima,atual->reta.p2.direita,atual->reta.p2.esquerda);
            }
        }
        //Direita
        if(atual->reta.p1.x >= mouseX + TOLERANCIA){
            if(atual->reta.p1.y >= (700 - mouseY) - TOLERANCIA && atual->reta.p1.y <= (700 - mouseY) + TOLERANCIA){
                atual->reta.p1.aBaixo = 0;
                atual->reta.p1.aCima= 0;
                atual->reta.p1.direita = 1;
                atual->reta.p1.esquerda = 0;
                printf("Quadrante direita p1, codigo %d%d%d%d\n", atual->reta.p1.aBaixo,atual->reta.p1.aCima,atual->reta.p1.direita,atual->reta.p1.esquerda);
            }
        }
        if(atual->reta.p2.x >= mouseX + TOLERANCIA){
            if(atual->reta.p2.y >= (700 - mouseY) - TOLERANCIA && atual->reta.p2.y <= (700 - mouseY) + TOLERANCIA){
                atual->reta.p2.aBaixo = 0;
                atual->reta.p2.aCima= 0;
                atual->reta.p2.direita = 1;
                atual->reta.p2.esquerda = 0;
                printf("Quadrante direita p2, codigo %d%d%d%d\n", atual->reta.p2.aBaixo,atual->reta.p2.aCima,atual->reta.p2.direita,atual->reta.p2.esquerda);
            }
        }
        //Baixo esquerda
        if(atual->reta.p1.x <= mouseX - TOLERANCIA && atual->reta.p1.y <= (700 - mouseY) - TOLERANCIA){
            atual->reta.p1.aBaixo = 1;
            atual->reta.p1.aCima= 0;
            atual->reta.p1.direita = 0;
            atual->reta.p1.esquerda = 1;
            printf("Quadrante baixo esquerda p1, codigo %d%d%d%d\n", atual->reta.p1.aBaixo,atual->reta.p1.aCima,atual->reta.p1.direita,atual->reta.p1.esquerda);
        }
        if(atual->reta.p2.x <= mouseX - TOLERANCIA && atual->reta.p2.y <= (700 - mouseY) - TOLERANCIA){
            atual->reta.p2.aBaixo = 1;
            atual->reta.p2.aCima= 0;
            atual->reta.p2.direita = 0;
            atual->reta.p2.esquerda = 1;
            printf("Quadrante baixo esquerda p2, codigo %d%d%d%d\n", atual->reta.p2.aBaixo,atual->reta.p2.aCima,atual->reta.p2.direita,atual->reta.p2.esquerda);
        }
        //Baixo
        if(atual->reta.p1.x >= mouseX - TOLERANCIA && atual->reta.p1.x <= mouseX + TOLERANCIA){
            if(atual->reta.p1.y <= (700 - mouseY) - TOLERANCIA){
                atual->reta.p1.aBaixo = 1;
                atual->reta.p1.aCima= 0;
                atual->reta.p1.direita = 0;
                atual->reta.p1.esquerda = 0;
                printf("Quadrante baixo p1, codigo %d%d%d%d\n", atual->reta.p1.aBaixo,atual->reta.p1.aCima,atual->reta.p1.direita,atual->reta.p1.esquerda);
            }
        }
        if(atual->reta.p2.x >= mouseX - TOLERANCIA && atual->reta.p2.x <= mouseX + TOLERANCIA){
            atual->reta.p2.aBaixo = 1;
            atual->reta.p2.aCima= 0;
            atual->reta.p2.direita = 0;
            atual->reta.p2.esquerda = 0;
            printf("Quadrante baixo p2, codigo %d%d%d%d\n", atual->reta.p2.aBaixo,atual->reta.p2.aCima,atual->reta.p2.direita,atual->reta.p2.esquerda);
        }
        //Baixo direita
        if(atual->reta.p1.x >= mouseX + TOLERANCIA && atual->reta.p1.y <= (700 - mouseY) - TOLERANCIA){
            atual->reta.p1.aBaixo = 1;
            atual->reta.p1.aCima= 0;
            atual->reta.p1.direita = 1;
            atual->reta.p1.esquerda = 0;
            printf("Quadrante baixo direita p1, codigo %d%d%d%d\n", atual->reta.p1.aBaixo,atual->reta.p1.aCima,atual->reta.p1.direita,atual->reta.p1.esquerda);
        }
        if(atual->reta.p2.x >= mouseX + TOLERANCIA && atual->reta.p2.y <= (700 - mouseY) - TOLERANCIA){
            atual->reta.p2.aBaixo = 1;
            atual->reta.p2.aCima= 0;
            atual->reta.p2.direita = 1;
            atual->reta.p2.esquerda = 0;
            printf("Quadrante baixo direita p2, codigo %d%d%d%d\n", atual->reta.p2.aBaixo,atual->reta.p2.aCima,atual->reta.p2.direita,atual->reta.p2.esquerda);
        }
        atual = atual->proximo;
    }
}

void alterar_posicao(ListaPonto* lista, Ponto p){
    NoPonto* atual = lista->inicio;
    while(atual != NULL){
        if(p.maximoX == atual->ponto.maximoX && p.maximoY == atual->ponto.maximoY && p.minimoX == atual->ponto.minimoX && p.minimoY == atual->ponto.minimoY){
            atual->ponto.x = p.x;
            atual->ponto.y = p.y;
        }
        atual = atual->proximo;
    }
}

void alterarPosicaoReta(Lista_reta* lista, Reta r){
    No* atual = lista->inicio;
    while(atual != NULL){
        if(r.p1.maximoX == atual->reta.p1.maximoX && r.p1.minimoX == atual->reta.p1.minimoX && r.p1.maximoY == atual->reta.p1.maximoY && r.p1.minimoY== atual->reta.p1.minimoY){
            if(r.p2.maximoX == atual->reta.p2.maximoX && r.p2.minimoX == atual->reta.p2.minimoX && r.p2.maximoY == atual->reta.p2.maximoY && r.p2.minimoY == atual->reta.p2.minimoY){
                atual->reta.p1.x = r.p1.x;
                atual->reta.p1.y = r.p1.y;
                atual->reta.p2.x = r.p2.x;
                atual->reta.p2.y = r.p2.y;
            }
        }
        atual = atual->proximo;
    }
}

void imprimir_lista_ponto(ListaPonto* lista){
    NoPonto* no_atual = lista->inicio;
    while (no_atual != NULL) {
        printf("(%d.%d)", no_atual->ponto.x, no_atual->ponto.y);
        no_atual = no_atual->proximo;
    }
    printf("\n");
}

void imprimir_lista_reta(Lista_reta* lista){
    No* atual = lista->inicio;
    while(atual != NULL) {
        printf("(%d,%d \n)", atual->reta.p1.x, atual->reta.p1.y);
        printf("(%d,%d \n)", atual->reta.p2.x, atual->reta.p2.y);
        atual = atual->proximo;
    }
}
