#!/bin/bash
if [ "$#" -gt 0 ]
    then
        if [ "$1" -eq 1 ]
            then
                echo 'compilando... '
                cd Codificador/
                ./compile_linux
                echo 'compilado!'
                shift
                echo 'carregando...'
                ./loader $1
                SAIDA=saida.bmp
                if [[ -f "$SAIDA" ]]; 
                    then
                        echo $PWD
                        rm -rf ../Decodificador/saida.bmp
                        echo $PWD
                        mv saida.bmp ../Decodificador/
                        echo 'Concluído com sucesso!'
                    else
                        echo 'Erro na execução do programa!'
                fi
                cd ..
            else
                echo 'compilando... '
                cd Decodificador/
                ./compile_linux
                echo 'compilado!'
                echo 'carregando...'
                ./loader saida.bmp

        fi
    else
        echo '==========================================='
        echo '|| Digite o código da opção desejada:    ||'
        echo '|| Codificar   => $1 <IMAGEM>            ||'
        echo '|| Decodificar => $2 ________            ||'
        echo '==========================================='
fi
