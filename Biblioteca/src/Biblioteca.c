#include <stdio.h>
#include <stdlib.h>
#include <mysql/mysql.h>

#define DB_HOST "localhost"
#define DB_USER "root"
#define DB_PASS "Lvc1998$"
#define DB_NAME "Biblioteca"

MYSQL *conn;

void finish_with_error(MYSQL *conn) {
    fprintf(stderr, "%s\n", mysql_error(conn));
    mysql_close(conn);
    exit(1);
}

void connect_to_database() {
    conn = mysql_init(NULL);
    if (conn == NULL) {
        fprintf(stderr, "mysql_init() failed\n");
        exit(1);
    }

    if (mysql_real_connect(conn, DB_HOST, DB_USER, DB_PASS, DB_NAME, 0, NULL, 0) == NULL) {
        finish_with_error(conn);
    }
}

void close_database() {
    mysql_close(conn);
}

void add_livro() {
    char titulo[100], autor[100], genero[50];
    int ano;

    printf("Digite o título do livro: ");
    scanf(" %[^\n]%*c", titulo);

    printf("Digite o autor do livro: ");
    scanf(" %[^\n]%*c", autor);

    printf("Digite o ano de publicação: ");
    scanf("%d", &ano);

    printf("Digite o gênero do livro: ");
    scanf(" %[^\n]%*c", genero);

    char query[256];
    sprintf(query, "INSERT INTO Livros (titulo, autor, ano, genero) VALUES ('%s', '%s', %d, '%s')",
            titulo, autor, ano, genero);

    if (mysql_query(conn, query)) {
        finish_with_error(conn);
    }

    printf("Livro adicionado com sucesso!\n");
}

void ver_livro() {
    if (mysql_query(conn, "SELECT * FROM Livros")) {
        finish_with_error(conn);
    }

    MYSQL_RES *result = mysql_store_result(conn);

    if (result == NULL) {
        finish_with_error(conn);
    }

    int num_fields = mysql_num_fields(result);
    MYSQL_ROW row;

    while ((row = mysql_fetch_row(result))) {
        for (int i = 0; i < num_fields; i++) {
            printf("%s ", row[i] ? row[i] : "NULL");
        }
        printf("\n");
    }

    mysql_free_result(result);
}

void atualizar_livro() {
    int id;
    char titulo[100], autor[100], genero[50];
    int ano;

    printf("Digite o ID do livro a ser atualizado: ");
    scanf("%d", &id);

    printf("Digite o novo título do livro: ");
    scanf(" %[^\n]%*c", titulo);

    printf("Digite o novo autor do livro: ");
    scanf(" %[^\n]%*c", autor);

    printf("Digite o novo ano de publicação: ");
    scanf("%d", &ano);

    printf("Digite o novo gênero do livro: ");
    scanf(" %[^\n]%*c", genero);

    char query[256];
    sprintf(query, "UPDATE Livros SET titulo='%s', autor='%s', ano=%d, genero='%s' WHERE id=%d",
            titulo, autor, ano, genero, id);

    if (mysql_query(conn, query)) {
        finish_with_error(conn);
    }

    printf("Livro atualizado com sucesso!\n");
}

void deletar_livro() {
    int id;
    printf("Digite o ID do livro a ser excluído: ");
    scanf("%d", &id);

    char query[256];
    sprintf(query, "DELETE FROM Livros WHERE id=%d", id);

    if (mysql_query(conn, query)) {
        finish_with_error(conn);
    }

    printf("Livro excluído com sucesso!\n");
}

int main() {

    connect_to_database();

    int opcao;

    while (1) {
        printf("\nSistema de Biblioteca\n");
        printf("1. Adicionar Livro\n");
        printf("2. Visualizar Livros\n");
        printf("3. Atualizar Livro\n");
        printf("4. Excluir Livro\n");
        printf("5. Sair\n");
        printf("Escolha uma opção: ");
        scanf("%d", &opcao);

        switch (opcao) {
            case 1:
                add_livro();
                break;
            case 2:
                ver_livro();
                break;
            case 3:
                atualizar_livro();
                break;
            case 4:
                deletar_livro();
                break;
            case 5:
                close_database();
                exit(0);
            default:
                printf("Opção inválida!\n");
        }
    }

    return 0;
}
