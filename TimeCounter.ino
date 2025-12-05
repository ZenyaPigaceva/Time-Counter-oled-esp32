void DisplayPrint() {
    oleg.clear(); // Очистка дисплея перед отрисовкой

    // Задать масштаб текста и начальные параметры
    oleg.setScale(1);
    oleg.setCursor(0, 0);

    // Добавляем заголовок
    oleg.print("Time Passed:");

    // Разделяющая линия
    oleg.drawLine(0, 10, 128, 10, OLED_STROKE_FILL);

    // Отображение времени в годах, месяцах, днях
    oleg.setScale(2); // Установить увеличенный шрифт
    oleg.setCursor(0, 20);
    oleg.print(String(Years) + "y");

    oleg.setCursor(50, 20);
    oleg.print(String(Months) + "m");

    oleg.setCursor(100, 20);
    oleg.print(String(Days) + "d");

    // Отображение времени в часах, минутах, секундах
    oleg.setScale(1);
    oleg.setCursor(0, 50);
    oleg.print(String(Hours) + "h " + String(Minutes) + "m " + String(Seconds) + "s");

    // Рисуем декоративные элементы (например, рамку)
    oleg.drawRect(0, 0, 128, 64, OLED_STROKE_FILL);

    // Применяем изменения на дисплее
    oleg.update(); 
}