# Observações sobre a AVL

+ Cada nó guarda sua própria altura. Isso otimiza muito as operações pois faz
com que não seja necessário recalculá-la toda vez

+ ```comparacoes``` é um atributo da árvore. Assim, a árvore deve ser um parâmetro
para toda função que deseja incrementar as comparações

+ Estou contabilizando praticamente toda comparação, exceto comparações de ponteiros
(== NULL) e não relacionadas
