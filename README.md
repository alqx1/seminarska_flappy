# Flappy Bird
Rekreacija igre Flappy Bird za seminarsko nalogo pri računalništvu.

## Gradnja
Za ustvarjanje gradbene datoteke uporabiš program CMake. Naprej ustvari direktorij, kjer se bodo shranjevale vse datoteke za gradnjo (v primeru je to direktorij "build" na operacijskem sistemu Linux), nato se vanj premakni in zaženi CMake:
    mkdir build
    cd build
    cmake -G<generator> ..
Ko se konča generacija vaše gradbene datoteke, zaženite program za grajenje (make, ninja ...):
    make
Končni program bo zgrajen v direktoriju bin. S tam zaženite program za pravilno delovanje

## Uporaba programa
Program oponaša delovanje igre Flappy Bird na telefonu. Uporabnik je ob zagonu predstavljen z opcijami za začetek igre ali izid programa. Ko uporabnik začne igro, ima nekaj časa za se pripraviti. Igro začne s klikom na ali levim klikom ali preslednico. Uporabnik se poskuša izogibati oviram s klikom na preslednico ali levim klikom na miško. Ko zadane oviro se igra konča. Uporabnik lahko ponovno igra s klikom na ustrezen gumb.

## Izgled programa
!(./readme/starting_scene.png "Starting scene")
!(./readme/gamescene.png "Game scene")
!(./readme/gameover.png "Gameover scene")
