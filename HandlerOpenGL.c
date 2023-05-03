#include <windows.h>
#include <GL/glut.h>
#include <stdio.h>
#include <math.h>
#include "Listas.c"
#include "VariaveisGlobais.h"
#define TOLERANCIA 25

typedef struct centroide{
    int x;
    int y;
}Centroide;

Ponto p1, p2, ponto_Poligono, pontoEscolhido;
Ponto pontoCimaEsquerda,pontoCima,pontoCimaDireita,pontoEsquerda,pontoCentro,pontoDireita,pontoBaixoEsquerda,pontoBaixo,pontoBaixoDireita;
Reta retaEscolhida;
ListaPonto *poligonoEscolhido;
Centroide cent;

int contadorVetorPoligonos = 0;
int opcaoEscolhida = 0; //Variavel que vai checar o menu escolhido
int contadorClicks = 0;
float angulo = 0.0;
int estadoModificacao = 0;
int intersecao = 0;

int ultimaPosXMouse, ultimaPosYMouse;

int comecouDesenhar = 0;

void criarPontos(float posX, float posY){
    Ponto p1 = {posX, posY, posX - TOLERANCIA, posX + TOLERANCIA, posY - TOLERANCIA, posY + TOLERANCIA};

    inserir_elemento_ponto(listaPontos, p1);

    printf("X: %d, minimoX: %d, maximoX: %d \n", p1.x, p1.minimoX, p1.maximoX);
    printf("Y: %d, minimoY: %d, maximoY: %d \n", p1.y, p1.minimoY, p1.maximoY);
}

void criarRetas(Ponto p1, Ponto p2){
    Reta reta;
    reta.p1.x = p1.x;
    reta.p1.y = p1.y;
    reta.p1.maximoX = p1.maximoX;
    reta.p1.minimoX = p1.minimoX;
    reta.p1.maximoY = p1.maximoY;
    reta.p1.minimoY = p1.minimoY;
    reta.p2.x = p2.x;
    reta.p2.y = p2.y;
    reta.p2.maximoX = p2.maximoX;
    reta.p2.minimoX = p2.minimoX;
    reta.p2.maximoY = p2.maximoY;
    reta.p2.minimoY = p2.minimoY;

    inserir_elemento_reta(listaReta, reta);

    printf("Ponto p1: %d, %d \n", p1.x, p1.y);
    printf("Ponto p2: %d, %d \n", p2.x, p2.y);

    glColor3f(0.5, 0.5, 0.9);
    glBegin(GL_LINES);
    glVertex2i(p1.x, p1.y);
    glVertex2i(p2.x, p2.y);
    glEnd();
}

void pegarPontosPoligonos(int x, int y){
    Ponto p = {x, y, x - TOLERANCIA, x + TOLERANCIA, y - TOLERANCIA, y + TOLERANCIA};
    printf("X: %d \n", p.x);
    printf("Y: %d \n", p.y);
    inserir_elemento_ponto(listaPoligono,p);

}

void moverPonto(int posX, int posY){
    if(opcaoEscolhida == 3){
        pontoEscolhido.x = posX;
        pontoEscolhido.y = 700 - posY;
        alterar_posicao(listaPontos, pontoEscolhido);

        printf("(%d,%d)\n", pontoEscolhido.x, pontoEscolhido.y);
        glutPostRedisplay();
    }
}

void rotacionarPonto(int posX, int posY, int sinal){
    //angulo = 0;
    if(sinal == 0){
        angulo = 1.0;
    }
    else if(sinal == 1) {
        angulo = -1.0;
    }

    float theta = (angulo * M_PI) / 180.0;
    pontoEscolhido.x = (posX * cos(theta)) - (posY * sin(theta));
    pontoEscolhido.y = (posX * sin(theta)) + (posY * cos(theta));
    alterar_posicao(listaPontos, pontoEscolhido);
    //printf("Valor inteiro: %d, %d\n", pontoEscolhido.x, pontoEscolhido.y);
    glutPostRedisplay();
}

int calcularAND(Ponto p1, Ponto p2){
    /*0: a baixo
      1: a cima
      2: direita
      3: esquerda*/
    int resultadoAND[4];
    int i;
    if(p1.aBaixo == 1 && p2.aBaixo == 1){
        resultadoAND[0] = 1;
    }
    else{
        resultadoAND[0] = 0;
    }
    if(p1.aCima == 1 && p2.aCima == 1){
        resultadoAND[1] = 1;
    }
    else{
        resultadoAND[1] = 0;
    }
    if(p1.direita == 1 && p2.direita == 1){
        resultadoAND[2] = 1;
    }
    else{
        resultadoAND[2] = 0;
    }
    if(p1.esquerda == 1 && p2.esquerda == 1){
        resultadoAND[3] = 1;
    }
    else{
        resultadoAND[3] = 0;
    }
    printf("Resposta AND\n");
    for(int i = 0;i < 4;i++){
        printf("%d ", resultadoAND[i]);
        if(resultadoAND[i] == 1){
            return 0;
        }
    }
    return 1;
}

Reta selecionarReta(Lista_reta* lista){
    int respostaAND = 0;
    No* atual = lista->inicio;
    while(atual != NULL){
        //printf("Checando reta\n");
        respostaAND = calcularAND(atual->reta.p1, atual->reta.p2);
        if(respostaAND == 1){
            //printf("Achei uma reta codigo %d%d%d%d \n", atual->reta.p1.aBaixo,atual->reta.p1.aCima,atual->reta.p1.direita,atual->reta.p1.esquerda);
            //printf("%d %d \n", atual->reta.p1.maximoX, atual->reta.p1.minimoX);
            return atual->reta;
        }
        atual = atual->proximo;
    }
}

void moverReta(int posX, int posY){
    if(opcaoEscolhida == 6){

        int centroRetaX = (retaEscolhida.p1.x + retaEscolhida.p2.x)/2;
        int centroRetaY = (retaEscolhida.p1.y + retaEscolhida.p2.y)/2;
        int dx = posX - centroRetaX;
        int dy = posY - centroRetaY;
        printf("PosX: %d, PosY: %d, ultimaPosXMouse: %d, dx: %d \n", posX, posY, ultimaPosXMouse, dx);

        retaEscolhida.p1.x += dx;
        retaEscolhida.p1.y += dy;
        retaEscolhida.p2.x += dx;
        retaEscolhida.p2.y += dy;


        printf("P1(%d,%d)\n", retaEscolhida.p1.x, retaEscolhida.p1.y);
        alterarPosicaoReta(listaReta,retaEscolhida);

        ultimaPosXMouse = posX;
        ultimaPosYMouse = posY;

        glutPostRedisplay();
    }
}

void rotacionarReta(int sinal){
    //angulo = 0;
    int pontoCentralX = (retaEscolhida.p1.x + retaEscolhida.p2.x)/2;
    int pontoCentralY = (retaEscolhida.p1.y + retaEscolhida.p2.y)/2;
    int p1XRelativo, p1YRelativo, p2XRelativo, p2YRelativo;

    //printf("Ponto central X: %d, Y: %d\n", pontoCentralX, pontoCentralY);
    if(sinal == 0){
        angulo = 5.0;
    }
    else if(sinal == 1) {
        angulo = -5.0;
    }
    float theta = (angulo * M_PI) / 180.0;

    p1XRelativo = retaEscolhida.p1.x - pontoCentralX;
    p1YRelativo = retaEscolhida.p1.y - pontoCentralY;
    p2XRelativo = retaEscolhida.p2.x - pontoCentralX;
    p2YRelativo = retaEscolhida.p2.y - pontoCentralY;

    retaEscolhida.p1.x = pontoCentralX + p1XRelativo*cos(theta) - p1YRelativo*sin(theta);
    retaEscolhida.p1.y = pontoCentralY + p1XRelativo*sin(theta) + p1YRelativo*cos(theta);
    retaEscolhida.p2.x = pontoCentralX + p2XRelativo*cos(theta) - p2YRelativo*sin(theta);
    retaEscolhida.p2.y = pontoCentralY + p2XRelativo*sin(theta) + p2YRelativo*cos(theta);

    /*printf("Antes da alteracao P1 x: %d x2: %d\n", retaEscolhida.p1.x, retaEscolhida.p1.y);
    retaEscolhida.p1.x = ((retaEscolhida.p1.x*cos(theta)) - (retaEscolhida.p1.y*sin(theta))) - ((pontoCentralX*cos(theta))+(sin(theta)*pontoCentralY)) + pontoCentralX;
    retaEscolhida.p1.y = ((retaEscolhida.p1.x*sin(theta)) + (retaEscolhida.p1.y*cos(theta))) - ((pontoCentralX*sin(theta))-(cos(theta)*pontoCentralY)) + pontoCentralY;
    printf("P1 x: %d y1: %d\n", retaEscolhida.p1.x, retaEscolhida.p1.y);
    retaEscolhida.p2.x = ((retaEscolhida.p2.x*cos(theta)) - (retaEscolhida.p2.y*sin(theta))) - ((pontoCentralX*cos(theta))+(sin(theta)*pontoCentralY)) + pontoCentralX;
    retaEscolhida.p2.y = ((retaEscolhida.p2.x*sin(theta)) + (retaEscolhida.p2.y*cos(theta))) - ((pontoCentralX*sin(theta))-(cos(theta)*pontoCentralY)) + pontoCentralY;*/

    alterarPosicaoReta(listaReta, retaEscolhida);
    glutPostRedisplay();
}

void escalonarReta(int sinal){
    float escala;
    int pontoCentralX = (retaEscolhida.p1.x + retaEscolhida.p2.x)/2;
    int pontoCentralY = (retaEscolhida.p1.y + retaEscolhida.p2.y)/2;
    if(sinal == 0){
        printf("Diminuindo reta\n");
        escala = 0.9;
    }
    else if(sinal == 1){
        printf("Aumentando reta\n");
        escala = 1.1;
    }
    retaEscolhida.p1.x = pontoCentralX - (pontoCentralX*escala) + (retaEscolhida.p1.x*escala);
    retaEscolhida.p1.y = pontoCentralY - (pontoCentralY*escala) + (retaEscolhida.p1.y*escala);
    retaEscolhida.p2.x = pontoCentralX - (pontoCentralX*escala) + (retaEscolhida.p2.x*escala);
    retaEscolhida.p2.y = pontoCentralY - (pontoCentralY*escala) + (retaEscolhida.p2.y*escala);
    alterarPosicaoReta(listaReta, retaEscolhida);
    glutPostRedisplay();
}

ListaPonto* checarMouseDentroPoligono(ListaPonto** lista, int posX, int posY){
    Ponto p1, p2;
    int contador = 0, quantidadeAretas = 0;
    //printf("Posicao mouse: (%d,%d)\n", posX, posY);
    while(contador < 10){
        NoPonto* no_inicio = (*(lista + 0))->inicio;
        if(listaTodosPoligonos[contador] != NULL){
            NoPonto* no_atual = (*(lista + contador))->inicio;
            while(no_atual != NULL){
                p1 = no_atual->ponto;
                if(no_atual->proximo != NULL){
                    p2 = no_atual->proximo->ponto;
                }
                else if(p1.x == p2.x && p1.y == p2.y){
                    p2 = no_inicio->ponto;
                }
                printf("P1(%d,%d)\n", p1.x, p1.y);
                printf("P2(%d,%d)\n", p2.x, p2.y);
                if(((p1.y > posY) && (p2.y > posY)) || ((p1.y < posY) && (p2.y < posY)) || ((p1.x < posX) && (p2.x < posX)) || (p1.y == p2.y)){
                    printf("-----------------------\n");
                }
                else if(((p1.x > posX) && (p2.x > posX)) && ((p1.y > posY) && (p2.y < posY))||((p1.y < posY) && (p2.y > posY))){
                    printf("Achou uma aretas\n");
                    quantidadeAretas += 1;
                }
                else{
                    intersecao = p1.x + ((posY - p1.y) * (p2.x - p1.x)) / (p2.y - p1.y);

                    if(intersecao > posX) quantidadeAretas += 1;
                }
                printf("Quantidade arestas: %d\n", quantidadeAretas);
                no_atual = no_atual->proximo;
            }
            if(quantidadeAretas %2 != 0){
                printf("selecionei\n ");
                return (*(lista + contador));
            }
        }
        contador += 1;
    }
    if(quantidadeAretas % 2 == 0){
        printf("Nao Selecionei\n");
        return NULL;
    }
}

Centroide calcularCentroide(ListaPonto* lista){
    Centroide resultado;
    int centroideX = 0, centroideY = 0, contadorVertices = 0;
    Ponto p1;
    NoPonto* atual = poligonoEscolhido->inicio;

    while(atual != NULL){
        p1 = atual->ponto;
        centroideX += p1.x;
        //printf("CentroideX: %d\n", centroideX);
        centroideY += p1.y;
        //printf("CentroideY: %d\n", centroideY);
        contadorVertices++;
        atual = atual->proximo;
    }
    resultado.x = centroideX/contadorVertices;
    resultado.y = centroideY/contadorVertices;

    return resultado;
}

void moverPoligono(int posX, int posY){
    if(opcaoEscolhida == 10 && poligonoEscolhido != NULL){
        printf("Ponto mouse (%d,%d)\n", posX, posY);
        cent = calcularCentroide(poligonoEscolhido);
        printf("Centroide: (%d,%d)\n", cent.x, cent.y);
        int dx = posX - cent.x;
        int dy = posY - cent.y;
        NoPonto* no_atual = poligonoEscolhido->inicio;
        printf("Estou movendo\n");
        while(no_atual != NULL){
            no_atual->ponto.x += dx;
            no_atual->ponto.y += dy;

            printf("(%d,%d)\n", no_atual->ponto.x, no_atual->ponto.y);

            /*ultimaPosXMouse = posX;
            ultimaPosYMouse = posY;*/

            no_atual = no_atual->proximo;
        }
        glutPostRedisplay();
    }
}

void rotacionarPoligono(int sinal){

    Ponto p1, p2;
    int p1XRelativo, p1YRelativo, p2XRelativo, p2YRelativo;

    NoPonto* atual = poligonoEscolhido->inicio;
    NoPonto* pontoInicial = poligonoEscolhido->inicio;
    cent = calcularCentroide(poligonoEscolhido);
    if(sinal == 0){
        angulo = 5.0;
    }
    else if(sinal == 1) {
        angulo = -5.0;
    }
    float theta = (angulo * M_PI) / 180.0;

    while(atual != NULL){
        p1 = atual->ponto;
        if(atual->proximo != NULL){
            p2 = atual->proximo->ponto;
        }
        else if(p1.x == p2.x && p1.y == p2.y){

            p2 = pontoInicial->ponto;
        }
        printf("Antes P1(%d,%d)\n", p1.x, p1.y);
        printf("Antes P2(%d,%d)\n", p2.x, p2.y);

        int pontoCentralX = (p1.x + p2.x)/2;
        int pontoCentralY = (p1.y + p2.y)/2;

        p1XRelativo = p1.x - cent.x;
        p1YRelativo = p1.y - cent.y;

        printf("Pontocentral (%d,%d)\n", pontoCentralX, pontoCentralY);

        p1.x = cent.x + p1XRelativo*cos(theta) - p1YRelativo*sin(theta);
        p1.y = cent.y + p1XRelativo*sin(theta) + p1YRelativo*cos(theta);

        alterar_posicao(poligonoEscolhido,p1);

        atual = atual->proximo;
    }

    glutPostRedisplay();

}

void escalonarPoligono(int sinal){
    Ponto p1, p2;
    float escala;
    NoPonto* atual = poligonoEscolhido->inicio;
    Ponto pontoInicial = poligonoEscolhido->inicio->ponto;
    //printf("Ponto inicial (%d,%d)\n", pontoInicial.x, pontoInicial.y);
    cent = calcularCentroide(poligonoEscolhido);
    if(sinal == 0){
        //printf("Diminuindo reta\n");
        escala = 0.9;
    }
    else if(sinal == 1){
        //printf("Aumentando reta\n");
        escala = 1.1;
    }

    while(atual != NULL){
        p1 = atual->ponto;
        if(atual->proximo != NULL){
            p2 = atual->proximo->ponto;
        }
        else if(p1.x == p2.x && p1.y == p2.y){
            p2 = pontoInicial;
        }

        int pontoX = p1.x;
        int pontoY = p1.y;

        p1.x = (pontoX*escala) - (cent.x*escala) + cent.x;
        p1.y = (pontoY*escala) - (cent.y*escala) + cent.y;

        alterar_posicao(poligonoEscolhido,p1);
        atual = atual->proximo;
    }
    glutPostRedisplay();
}

void removerPoligono(ListaPonto* lista){
    NoPonto* atual = poligonoEscolhido->inicio;
    while(atual != NULL){
        remover_elemento_ponto(lista, atual->ponto);
        atual = atual->proximo;
    }
}

void mouseHandler(int button, int state, int mouseX, int mouseY){

    if (button == GLUT_LEFT_BUTTON && state == GLUT_DOWN) {
        int x = mouseX;
        int y = 700 - mouseY;
        printf("Opcao Escolhida %d\n", opcaoEscolhida);

        if(opcaoEscolhida == 0){
            printf("Ponto \n");
            criarPontos(x, y);
            glFlush();
        }
        else if(opcaoEscolhida == 1){
            printf("Reta \n");
            contadorClicks++;
            if(contadorClicks == 1){
                printf("Primeiro clique\n");
                p1.x = x;
                p1.y = y;
                p1.maximoX = x + TOLERANCIA;
                p1.minimoX = x - TOLERANCIA;
                p1.maximoY = y + TOLERANCIA;
                p1.minimoY = y - TOLERANCIA;
            }
            else if(contadorClicks == 2){
                printf("Segundo clique\n");
                p2.x = x;
                p2.y = y;
                p2.maximoX = x + TOLERANCIA;
                p2.minimoX = x - TOLERANCIA;
                p2.maximoY = y + TOLERANCIA;
                p2.minimoY = y - TOLERANCIA;
                criarRetas(p1, p2);
                contadorClicks = 0;
                glFlush();
            }
        }
        else if(opcaoEscolhida == 2){
            //printf("Poligono \n");
            if(comecouDesenhar == 0){
                listaPoligono = criar_lista_ponto();
                while(contadorVetorPoligonos < 10){
                    if(listaTodosPoligonos[contadorVetorPoligonos] == NULL){
                        *(listaTodosPoligonos + contadorVetorPoligonos) = listaPoligono;
                        break;
                    }
                    contadorVetorPoligonos += 1;
                }
                comecouDesenhar = 1;
            }
            pegarPontosPoligonos(x, y);
        }
        else if(opcaoEscolhida == 3){
            pontoEscolhido = acessar_hitbox_ponto(listaPontos, x, y);
            if(pontoEscolhido.minimoX != 0 && pontoEscolhido.maximoX != 0 && pontoEscolhido.minimoY != 0 && pontoEscolhido.maximoY != 0){
                printf("Ponto selecionadao: %d, %d, %d \n", pontoEscolhido.x, pontoEscolhido.y, pontoEscolhido.maximoX);
                glutMotionFunc(moverPonto);
                glFlush();
            }
            else{
                printf("Nao selecionou um ponto \n");
            }
        }
        else if(opcaoEscolhida == 4){
            printf("Rotacionando");
            pontoEscolhido = acessar_hitbox_ponto(listaPontos, x, y);
            if(pontoEscolhido.minimoX != 0 && pontoEscolhido.maximoX != 0 && pontoEscolhido.minimoY != 0 && pontoEscolhido.maximoY != 0 ){
                printf("Ponto selecionadao: %d, %d, %d \n", pontoEscolhido.x, pontoEscolhido.y, pontoEscolhido.maximoX);
                glFlush();
            }
            else{
                printf("Nao selecionou um ponto \n");
            }
        }
        else if(opcaoEscolhida == 5){
            printf("Removi\n");
            pontoEscolhido = acessar_hitbox_ponto(listaPontos, x, y);
            if(pontoEscolhido.minimoX != 0 && pontoEscolhido.maximoX != 0 && pontoEscolhido.minimoY != 0 && pontoEscolhido.maximoY != 0 ){
                printf("Ponto selecionadao: %d, %d, %d \n", pontoEscolhido.x, pontoEscolhido.y, pontoEscolhido.maximoX);
                remover_elemento_ponto(listaPontos, pontoEscolhido);
                glFlush();
            }
            else{
                printf("Nao selecionou um ponto \n");
            }
        }
        else if(opcaoEscolhida == 6){
            checarPontosRetas(listaReta, mouseX, mouseY);
            retaEscolhida = selecionarReta(listaReta);
            glutMotionFunc(moverReta);
            glFlush();
            //printf("Reta escolhida: p1(%d,%d) p2(%d,%d)\n", retaEscolhida.p1.x, retaEscolhida.p1.y, retaEscolhida.p2.x, retaEscolhida.p2.y);
        }
        else if(opcaoEscolhida == 7){
            printf("Rotacionando\n");
            checarPontosRetas(listaReta, mouseX, mouseY);
            retaEscolhida = selecionarReta(listaReta);

        }
        else if(opcaoEscolhida == 8){
            printf("Escaloandno\n");
            checarPontosRetas(listaReta, mouseX, mouseY);
            retaEscolhida = selecionarReta(listaReta);
        }
        else if(opcaoEscolhida == 9){
            printf("Removendo reta\n");
            checarPontosRetas(listaReta, mouseX, mouseY);
            retaEscolhida = selecionarReta(listaReta);
            remover_elemento_reta(listaReta,retaEscolhida);
            glFlush();
        }
        else if(opcaoEscolhida == 10){
            poligonoEscolhido = checarMouseDentroPoligono(listaTodosPoligonos, x, y);
            if(poligonoEscolhido != NULL){
                imprimir_lista_ponto(poligonoEscolhido);
                glutMotionFunc(moverPoligono);
                glFlush();
            }
        }
        else if(opcaoEscolhida == 11){
            printf("Rotacionando poligono\n");
            poligonoEscolhido = checarMouseDentroPoligono(listaTodosPoligonos, x, y);
        }
        else if(opcaoEscolhida == 12){
            printf("Escalonar o poligono\n");
            poligonoEscolhido = checarMouseDentroPoligono(listaTodosPoligonos, x, y);
        }
        else if(opcaoEscolhida == 13){
            poligonoEscolhido = checarMouseDentroPoligono(listaTodosPoligonos, x, y);
            if(poligonoEscolhido != NULL){
                removerPoligono(poligonoEscolhido);
            }
        }
        //printf("X: %d \n", x);
        //printf("Y: %d \n", y);
    }
    else if(button == GLUT_LEFT_BUTTON && state == GLUT_UP){
        //printf("Soltei");
        recalcularHitBox(listaPontos);
    }
    else if(button == GLUT_MIDDLE_BUTTON && state == GLUT_DOWN){
        imprimir_lista_ponto(poligonoEscolhido);
        //imprimir_lista_reta(listaReta);
        //imprimir_lista_poligono(poligono);
    }
}

void keyboardHandler(unsigned char key, int x, int y){
    if(key == 'd' && opcaoEscolhida == 2){
        comecouDesenhar = 0;
    }
    //Ponto
    else if(opcaoEscolhida == 4){
        if(key == 'w'){
            printf("Subindo\n");
            rotacionarPonto(pontoEscolhido.x, pontoEscolhido.y, 0);
        }
        else if(key == 's'){
            printf("Descendo\n");
            rotacionarPonto(pontoEscolhido.x, pontoEscolhido.y, 1);
        }
    }
    //Reta
    else if(opcaoEscolhida == 7){
        if(key == 'w'){
            printf("Subindo\n");
            rotacionarReta(0);
        }
        else if(key == 's'){
            printf("Descendo\n");
            rotacionarReta(1);
        }
    }
    else if(opcaoEscolhida == 8){
        if(key == 'a'){
            escalonarReta(0);
        }
        else if(key == 'd'){
            escalonarReta(1);
        }
    }
    //Poligono
    else if(opcaoEscolhida == 11){
        if(poligonoEscolhido != NULL){
            if(key == 'w'){
                printf("Subindo\n");
                rotacionarPoligono(0);
        }
            else if(key == 's'){
                printf("Descendo\n");
                rotacionarPoligono(1);
            }
        }
    }
    else if(opcaoEscolhida == 12){
        if(poligonoEscolhido != NULL){
            if(key == 'a'){
                escalonarPoligono(0);
            }
            else if(key == 'd'){
                escalonarPoligono(1);
            }
        }
    }
}

void desenharPoligono(ListaPonto* listaPonto){
    glColor3f(0.5, 0.5, 0.9);
    //printf("Desenhei");
    glBegin(GL_POLYGON);
    NoPonto* no_atual = listaPonto->inicio;
     while(no_atual != NULL){
        glVertex2i(no_atual->ponto.x, no_atual->ponto.y);
        //printf("(%d, %d) ", no_atual->ponto.x, no_atual->ponto.y);
        no_atual = no_atual->proximo;
     }
     glEnd();
}

void opcoesFiguras(int id){
    opcaoEscolhida = id;

    glutPostRedisplay();
}

void menuFiguras(){
    int menu = glutCreateMenu(opcoesFiguras);

    glutAddMenuEntry("Ponto", 0);
    glutAddMenuEntry("Linha", 1);
    glutAddMenuEntry("Poligono", 2);
    glutAddMenuEntry("Mover ponto", 3);
    glutAddMenuEntry("Rotacionar ponto", 4);
    glutAddMenuEntry("Remover ponto", 5);
    glutAddMenuEntry("Mover reta", 6);
    glutAddMenuEntry("Rotacionar reta", 7);
    glutAddMenuEntry("Escalonar reta", 8);
    glutAddMenuEntry("Remover reta", 9);
    glutAddMenuEntry("Mover poligono", 10);
    glutAddMenuEntry("Rotacionar poligono", 11);
    glutAddMenuEntry("Escalonar poligono", 12);
    glutAddMenuEntry("Remover poligono", 13);

    glutAttachMenu(GLUT_RIGHT_BUTTON);
}

void redesenharPontos(ListaPonto* lista){
    NoPonto* atual = lista->inicio;
    glPointSize(10);
    glColor3f(0.5, 0.5, 0.9);
    glBegin(GL_POINTS);
    while(atual != NULL){
        glVertex2i(atual->ponto.x,atual->ponto.y);
        atual = atual->proximo;
    }
    glEnd();
}

void redesenharRetas(Lista_reta* lista){
    No* atual = lista->inicio;
    glLineWidth(3);
    glColor3f(0.5, 0.5, 0.9);
    glBegin(GL_LINES);
    while(atual != NULL){
        glVertex2i(atual->reta.p1.x,atual->reta.p1.y);
        glVertex2i(atual->reta.p2.x,atual->reta.p2.y);
        atual = atual->proximo;
    }
    glEnd();
}

void redesenharPoligonos(ListaPonto** lista){
    int contador = 0;
    while(contador < 10){
        if(listaTodosPoligonos[contador] != NULL){
            desenharPoligono(*(lista + contador));
        }
        contador += 1;
    }
}

void recalcularHitBox(ListaPonto* lista){
    NoPonto* atual = lista->inicio;
    while(atual != NULL){
        atual->ponto.minimoX = atual->ponto.x - 25;
        atual->ponto.maximoX = atual->ponto.x + 25;
        atual->ponto.minimoY = atual->ponto.y - 25;
        atual->ponto.maximoY = atual->ponto.y + 25;
        atual = atual->proximo;
    }
}
