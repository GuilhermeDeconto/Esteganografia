# Esteganografia


1 Introdução
Esteganografia é um método bastante usado para ocultar informações.  Ela consiste em aplicar técnicas capazes de ocultar informações dentro de outros arquivos como imagens, músicas ou vídeos ou mesmo anúncios de jornais e revistas. O objetivo deste trabalho é praticar a manipulação de memória em baixo nível, implementando uma técnica simples de esteganografia em imagens.

2 Funcionamento
Deverão ser criados dois programas:
Codificador: lê uma imagem, codifica uma mensagem nele, grava um novo arquivo com o resultado
Decodificador: lê a imagem com as informações ocultas, decodifica e exibe a mensagem na tela
Para ler as imagens utilizaremos uma biblioteca simples (integrada no projeto de exemplo) denominada SOIL.
Uma imagem é geralmente representada por uma matriz de pontos (pixels) onde cada cor é definida por 3 componentes: vermelho (R), verde (G) e azul (B). Cada uma dessas componentes usualmente é codificada em um byte, o que produz 3 bytes por pixel (24 bits) - ou seja, 16 milhões de possíveis cores. Em outras palavras, as intensidades (R, G, B) variam de 0 a 255, onde 0 é escuro e 255 é claro.

2.1 Codificador
O codificador deverá:
Ler a imagem de entrada
Solicitar uma senha qualquer
Solicitar a mensagem a ser armazenada
Aplicar uma função sobre a senha que permita definir um deslocamento inicial sobre o arquivo definindo o primeiro pixel para a codificação da mensagem e, ainda, um salto a ser aplicado após a codificação de cada caracter.
A senha deve ser utilziada, ainda, para aplicar algum algoritmo de cifragem da mensagem.
Utilizar a técnica de esteganografia explicada abaixo e incorporar a mensagem na imagem original
Salvar o arquivo resultante em formato BMP (formato suportado pela biblioteca SOIL)
para salvar a imagem com a biblioteca SOIL, usar (na main antes de liberrar a memória (free):
SOIL_save_image("saida.bmp", SOIL_SAVE_TYPE_BMP, pic.width, pic.height, 3, pic.img);

2.2 Decodificador
O decodificador deverá:
Ler a imagem com as informações ocultas em formato BMP
Solicitar a senha (mesma utilizada na etapa de codificação)
Recuperar os caracteres a partir do conteúdo da imagem
Reverter a cifragem e gerar a mensagem original, exibindo-a na tela

3 Implementando Esteganografia
Há diversas técnicas para realizar o processo de esteganografia em imagens, com graus diferentes de complexidade. Uma técnica simples é a alteração dos bits menos significativos de cada pixel. Essa técnica consiste em substituir um ou mais bits menos significativos de cada componente de cor de um pixel por uma parte da mensagem. Como temos 8 bits para R, G e B, isso em geral altera muito pouco a cor de cada pixel.
Por exemplo, se trocarmos os últimos dois bits de um pixel 100% vermelho (11111111) para 11111101, isso implica em alterar o valor da componente vermelha de 255 para 253, o que é praticamente imperceptível a olho nu.

