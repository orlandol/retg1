
// Generated on 1min.ai with Claude 4.5 Haiku - Anthropic

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX_EXPRS 100
#define MAX_VARS 50

typedef struct {
    char left[20];
    char right[100];
    int id;
    int first_occurrence;
} Expression;

typedef struct {
    Expression exprs[MAX_EXPRS];
    int count;
} ExpressionTable;

typedef struct {
    char name[20];
    int expr_id;
} VarMapping;

typedef struct {
    VarMapping mappings[MAX_VARS];
    int count;
} CSEContext;

int hash_expression(const char *expr) {
    int hash = 0;
    for (int i = 0; expr[i]; i++) {
        hash = (hash * 31 + expr[i]) % 10007;
    }
    return hash;
}

int find_expression(ExpressionTable *table, const char *expr) {
    for (int i = 0; i < table->count; i++) {
        if (strcmp(table->exprs[i].right, expr) == 0) {
            return i;
        }
    }
    return -1;
}

void add_expression(ExpressionTable *table, const char *left, const char *right, int line) {
    if (table->count >= MAX_EXPRS) return;

    strcpy(table->exprs[table->count].left, left);
    strcpy(table->exprs[table->count].right, right);
    table->exprs[table->count].id = table->count;
    table->exprs[table->count].first_occurrence = line;
    table->count++;
}

void eliminate_subexpressions(ExpressionTable *table, CSEContext *context) {
    printf("=== Common Subexpression Elimination ===\n\n");

    for (int i = 0; i < table->count; i++) {
        int duplicate_idx = find_expression(table, table->exprs[i].right);

        if (duplicate_idx < i) {
            printf("Line %d: %s = %s\n", i + 1, table->exprs[i].left, table->exprs[i].right);
            printf("  -> Reuse result from line %d: temp%d = %s\n",
                   duplicate_idx + 1, duplicate_idx, table->exprs[duplicate_idx].right);
            printf("  -> Optimized: %s = temp%d\n\n",
                   table->exprs[i].left, duplicate_idx);
        } else {
            printf("Line %d: %s = %s (first occurrence)\n", i + 1,
                   table->exprs[i].left, table->exprs[i].right);
            printf("  -> Create temp%d for reuse\n\n", i);
        }
    }
}

void print_optimization_report(ExpressionTable *table) {
    printf("\n=== Optimization Report ===\n");
    int eliminated = 0;

    for (int i = 0; i < table->count; i++) {
        for (int j = i + 1; j < table->count; j++) {
            if (strcmp(table->exprs[i].right, table->exprs[j].right) == 0) {
                eliminated++;
            }
        }
    }

    printf("Total expressions: %d\n", table->count);
    printf("Redundant computations eliminated: %d\n", eliminated);
    printf("Estimated optimization: %.1f%%\n",
           (eliminated * 100.0) / (table->count ? table->count : 1));
}

int main() {
    ExpressionTable table = {0};
    CSEContext context = {0};

    // Example: parsing compiler intermediate code
    add_expression(&table, "a", "x + y", 1);
    add_expression(&table, "b", "x + y", 2);  // Common subexpression
    add_expression(&table, "c", "a * 2", 3);
    add_expression(&table, "d", "x + y", 4);  // Another duplicate
    add_expression(&table, "e", "a * 2", 5);  // Another duplicate

    eliminate_subexpressions(&table, &context);
    print_optimization_report(&table);

    return 0;
}
