#include <windows.h>
#include <GL/glut.h>
#include "HandlerOpenGL.c"
#include "VariaveisGlobais.h"

int tamanhoListasPoligonos = 10;

int init(void) {
    glClearColor(1.0, 1.0, 1.0, 0.0);
    glMatrixMode(GL_PROJECTION);
    gluOrtho2D(0, 800, 0, 700);
}

void display(void) {
    glClear(GL_COLOR_BUFFER_BIT);
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();

    redesenharPontos(listaPontos);
    redesenharRetas(listaReta);
    redesenharPoligonos(listaTodosPoligonos);

    glutSwapBuffers();
}

int main(int argc, char** argv) {
    listaPontos = criar_lista_ponto();
    listaReta = criar_lista_reta();

    listaTodosPoligonos = (ListaPonto *)malloc(tamanhoListasPoligonos * sizeof(ListaPonto *));
    for (int i = 0; i < tamanhoListasPoligonos; ++i) {
        *(listaTodosPoligonos + i) = NULL;
    }

    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_DOUBLE| GLUT_RGB);
    glutInitWindowSize(800,700);
    glutInitWindowPosition(200,0);
    glutCreateWindow("Projeto CG");
    menuFiguras();

    glutMouseFunc(mouseHandler);
    glutKeyboardFunc(keyboardHandler);

    init();
    glutDisplayFunc(display);
    glutMainLoop();
    return 0;
}
