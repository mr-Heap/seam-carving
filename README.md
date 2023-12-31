# Seam carving

## Описание
**Seam carving** — это алгоритм для изменения размера изображения, который сохраняет важные элементы и удаляет менее значимые. Если смотреть на изображение как на таблицу из пикселей, то на каждой итерации данного алгоритма удаляется только один пиксель из колонки (или ряда).

Если переводить дословно, то “seam carving” это “резьба по шву”. Вертикальным “швом” изображения называется последовательность связанных пикселей от верхнего ряда до нижнего, где в каждом ряду только один пиксель. Аналогично, горизонтальным “швом” называется последовательность связанных пикселе от крайней левой колонки до крайней правой, где в каждой колонке только один пиксель.

После того, как алгоритм закончил работу, было убрано 150 вертикальных последовательностей пикселей, что помогло уменьшить изображение на *30%*. Стоит отметить, что в отличие от стандартных подходов, где изображение бы кадрировали или масштабировали с потерей пропорций, *seam carving* оставил набор важных объектов на фотографии нетронутыми. Алгоритм *seam carving* активно используется в современных графических редакторах.

Реализована структура данных, которая будет изменять размер изображения (WxH), использую алгоритм *seam carving*.

### Обозначения
Пиксель *(x, y)* в исходном изображении соответствует ячейке в таблице с колокой *x* и строкой *y*. Таким образом, пиксель *(0, 0)* находится в левом верхний углу таблицы, а пиксель *(W-1, H-1)* в правом нижнем углу таблицы.

### Алгоритм
Алгоритм вычисляет энергию каждого пикселя и удаляет шов с минимальной суммарной энергией.
### Вычисления
#### Энергия пикселя
Для вычисления энергии пикселя используется следующая формула:

<img src="https://render.githubusercontent.com/render/math?math=e=\sqrt{\Delta_{x}^{2}(x, y) %2B \Delta_{y}^{2}(x, y)}">, где <img src="https://render.githubusercontent.com/render/math?math=\Delta_{x}^{2}(x, y) = R_{x}^{2}(x,y) %2B G_{x}^{2}(x,y) %2B B_{x}^{2}(x,y)">

<img src="https://render.githubusercontent.com/render/math?math=R_{x}^{2}(x,y)"> это разность между компонентами пикселей *(x + 1, y)* и *(x - 1, y)*, отвечающими за красный цвет. Аналогично определяются <img src="https://render.githubusercontent.com/render/math?math=G_{x}^{2}(x,y)"> и <img src="https://render.githubusercontent.com/render/math?math=B_{x}^{2}(x,y)"> отвечающие за разность зеленых и голубых компонент соответственно.

Для вычисления <img src="https://render.githubusercontent.com/render/math?math=\Delta_{y}^{2}(x, y)"> необходимо рассматривать пиксели *(x, y + 1)* и *(x, y - 1)*.

Чтобы посчитать энергию пикселя *(0, y)*, который находится на границе изображения, нужно будет использовать пиксели *(1, y)* и *(W - 1, y)*.

Рассмотрим то, как вычислять энергию пикселей на примере изображения *3x4*:

![](data/text/energycalc.png)

Рассмотрим пиксель *(1, 2)*, который находится не на границе. Чтобы посчитать <img src="https://render.githubusercontent.com/render/math?math=\Delta_{x}"> нам потребуется рассмотреть пиксели *(0, 2)* и *(2, 2)*.

<img src="https://render.githubusercontent.com/render/math?math=R_{x}(1,2) = 255 %2D 255 = 0 ">

<img src="https://render.githubusercontent.com/render/math?math=G_{x}(1,2) = 205 %2D 203 = 2 ">

<img src="https://render.githubusercontent.com/render/math?math=B_{x}(1,2) = 255 %2D 51 = 204 ">

И получаем, что:

<img src="https://render.githubusercontent.com/render/math?math=\Delta^{2}_{x}(1, 2) = 2^2 %2B 204^2 = 41620">

Чтобы посчитать <img src="https://render.githubusercontent.com/render/math?math=\Delta_{y}"> нам потребуется рассмотреть пиксели *(1, 1)* и *(1, 3)*.

<img src="https://render.githubusercontent.com/render/math?math=R_{y}(1,2) = 255 %2D 255 = 0 ">

<img src="https://render.githubusercontent.com/render/math?math=G_{y}(1,2) = 205 %2D 153 = 102 ">

<img src="https://render.githubusercontent.com/render/math?math=B_{y}(1,2) = 153 %2D 153 = 0 ">

И

<img src="https://render.githubusercontent.com/render/math?math=\Delta^{2}_{y}(1, 2) = 102^2 = 10404">

Python-скрипты для работы с изображениями:

Пример использования:
1. Генерация `csv` файла по изображению
   ```
   python scripts/img_to_csv.py data/tower.jpeg data/tower.csv
   ```
2. Запуск алгоритма *seam-carving*
   ```
   ./seam-carving data/tower.csv data/tower_updated.csv    
   ```
3. Генерация изображения по `csv` файлу
   ```
   python scripts/csv_to_img.py data/tower_updated.csv data/tower_updated.jpeg 
   ```

Изображение `tower_updated.jpeg` — результат работы алгоритма *seam carving*.

Для запуска python скриптов потребуются 3-й python и пакеты:
* imageio
* numpy
* pandas
* argparse

