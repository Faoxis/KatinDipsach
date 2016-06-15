package com.faoxis.gui;

import com.faoxis.port.PortConnection;
import jssc.SerialPortException;
import org.jfree.chart.ChartFactory;
import org.jfree.chart.ChartPanel;
import org.jfree.chart.JFreeChart;
import org.jfree.chart.plot.PlotOrientation;
import org.jfree.data.xy.XYDataset;
import org.jfree.data.xy.XYSeries;
import org.jfree.data.xy.XYSeriesCollection;

import javax.swing.*;
import javax.swing.border.EmptyBorder;
import java.awt.*;
import java.awt.event.ActionEvent;
import java.awt.event.ActionListener;
import java.util.HashMap;

/**
 * Основной класс графического интерфейса.
 * Помимо графики он так же осуществляет построение графика и работу с com-портом
 * @author Sergei Samoilov
 * @version 0.1
 */
public class MainWindow {

    /** Определение порта для работы */
    private String workingPort;
    /** Объект управления com-портом */
    private PortConnection portConnection;
    /** Кнопка старт */
    private JButton startButton;
    /** Кнопка стоп */
    private JButton stopButton;
    /** Кнопка состояния графика */
    private boolean isStart;

    /** Настройка графика для первого канала */
    private XYSeries channel1;
    /** Настройка графика для второго канала */
    private XYSeries channel2;
    /** Счетчик пришедших байт */
    private int counter = 0;
    /** Настрока панели с графиком */
    private ChartPanel chartPanel;
    /** Состоние работы программы */
    private JLabel labelState = new JLabel();


    private JMenu portMenu;


    private GridBagConstraints getGridBagConstraints(int col, int row, int size) {
        GridBagConstraints layConstraints = new GridBagConstraints();
        layConstraints.fill = GridBagConstraints.BOTH; // заполняет ячейку целиком
        layConstraints.gridwidth = size;
        layConstraints.gridx = row; // координаты ячейки, в которую помещается кнопка
        layConstraints.gridy = col;
        return layConstraints;
    }



    /** Точка входа в программу
     * @see main
     */
    public static void main(String[] args) {
        new MainWindow().start();
    }

    /**
     * Тут у нас основная функция класса.
     * В ней строится основное окно программы целиком.
     */
    public void start() {

        // Временно задаем com-порт вручную
        //workingPort        = "COM5";


        JFrame frame = new JFrame("Dipsach_v0.11");
        frame.setDefaultCloseOperation(JFrame.EXIT_ON_CLOSE);


        //------------------------------- Вкладка с выбором порта ------------------------------------//
        portMenu = new JMenu("Порт");
        String[] portsName = PortConnection.getPorts();
        HashMap<String, JMenuItem> ports = new HashMap<>();

        for (int i = 0; i < portsName.length; ++i) {
            ports.put(portsName[i], new JMenuItem(portsName[i]));
            portMenu.add(ports.get(portsName[i]));
            ports.get(portsName[i]).addActionListener(new SelectPortsListener(portsName[i]));
        }

        JMenuItem refreshPorts = new JMenuItem("Обновить список com-портов");
        portMenu.add(refreshPorts);
        refreshPorts.addActionListener(new RefreshPortsListener());
        //----------------------------------------------------------------------------------------------//

        //------------------------------- Вкладка соединение -------------------------------------------//
        JMenuItem checkConnection = new JMenuItem("Проверить соединение");
        checkConnection.addActionListener(new CheckConnectionListener());
        JMenuItem openConnection = new JMenuItem("Подключиться");
        openConnection.addActionListener(new OpenConnectionListener());
        JMenuItem closeConnection = new JMenuItem("Отключиться");
        closeConnection.addActionListener(new CloseConnectionListener());
        JMenu connectionMenu = new JMenu("Соединение");
        connectionMenu.add(checkConnection);
        connectionMenu.add(openConnection);
        connectionMenu.add(closeConnection);
        //--------------------------------------------------------------------------------------------//


        JMenuBar menuBar = new JMenuBar();
//        menuBar.add(fileMenu);
        menuBar.add(portMenu);
        menuBar.add(connectionMenu);
        //********************************************************************************************//

        //***************************** Настройка информации о подключении снизу *********************//
        labelState = new JLabel();
        labelState.setText("Программа только что запустилась");
        //********************************************************************************************//


        //***************************** Настройка менюшки справа *************************************//
        GridBagConstraints layConstraints;

        JPanel eastPanel = new JPanel();
        eastPanel.setBorder(new EmptyBorder(4, 4, 4, 4)); // отступ внутри панели на 4 пикселя со всех сторон
        GridBagLayout layout = new GridBagLayout();
        layout.rowHeights = new int[]{23, 23, 23, 23, 23, 23, 23, 23, 23, 23, 23, 23, 23, 0}; // высоты кнопок
        layout.columnWeights = new double[]{1.0};
        layout.rowWeights = new double[14];
        layout.rowWeights[layout.rowWeights.length - 1] = Double.MIN_VALUE;
        eastPanel.setLayout(layout);


        startButton = new JButton("Старт");
        startButton.addActionListener(new StartButtonListener());
        layConstraints = getGridBagConstraints(0, 0, 1);
        eastPanel.add(startButton, layConstraints); // добавление кнопки на панель с учётом разметки

        stopButton = new JButton("Стоп");
        stopButton.addActionListener(new StopButtonListener());
        layConstraints = getGridBagConstraints(0, 1, 1);
        eastPanel.add(stopButton, layConstraints);


        JButton clearButton = new JButton("Очистить");
        clearButton.addActionListener(new ClearButtonListener());
        layConstraints = getGridBagConstraints(1, 0, 2);
        eastPanel.add(clearButton, layConstraints);


//        JButton setAngleButton = new JButton("Выставить угол:");
//        setAngleButton.addActionListener(new SetAngleButtonListener());
//        layConstraints = getGridBagConstraints(12, 0, 1);
//        eastPanel.add(setAngleButton, layConstraints);
//
//        JTextArea angleValueArea = new JTextArea();
//        angleValueArea.setFont(new Font("TimesRoman", Font.BOLD, 24));
//        layConstraints = getGridBagConstraints(12, 1, 1);
//        eastPanel.add(angleValueArea, layConstraints);
        //********************************************************************************************//


        //***************************** Настройка графика по центру **********************************//
        channel1 = new XYSeries("Канал 1");
        channel2 = new XYSeries("Канал 2");

        XYSeriesCollection dataset = new XYSeriesCollection();
        dataset.addSeries(channel1);
        //dataset.addSeries(channel2);

        XYDataset xyDataset = dataset;

//        JFreeChart chart = ChartFactory.createXYLineChart(
//                "Карта препятствий", "Угол ", "Дистанция ", xyDataset, PlotOrientation.VERTICAL, true, true, true);

        JFreeChart chart = ChartFactory.createPolarChart("Карта препятсвий", xyDataset, true, true, true);

        chartPanel = new ChartPanel(chart);
        //********************************************************************************************//


        frame.setSize(800, 500);
        frame.setMinimumSize(new Dimension(800, 500));
        frame.getContentPane().add(menuBar, BorderLayout.NORTH);
        frame.getContentPane().add(chartPanel, BorderLayout.CENTER);
        frame.getContentPane().add(eastPanel, BorderLayout.EAST);
        frame.getContentPane().add(labelState, BorderLayout.SOUTH);
        frame.setVisible(true);

    } // Конец метода start()


    /**
     * Класс для прослушивания кнопки "Проверить соединение"
     */
    private class CheckConnectionListener implements ActionListener {
        @Override
        public void actionPerformed(ActionEvent e) {
            try {
                portConnection = new PortConnection(workingPort);
                portConnection.closePortConnection();
                portConnection = null;

                labelState.setText("Соединение было успешно проверено.");
                labelState.repaint();
            } catch (SerialPortException ex) {
                labelState.setText("Проверка соединения закончилось неудачей. " +
                        "Возможно вы уже подключены к устройству или не выбрали порт.");
                labelState.repaint();
            }
        } // Конец метода actionPerformed
    } // Конец класса CheckConnectionListener

    /**
     * Класс для прослушивания кнопки "Подключиться"
     */
    private class OpenConnectionListener implements ActionListener {
        @Override
        public void actionPerformed(ActionEvent e) {
            try {
                if (portConnection == null) {
                    portConnection = new PortConnection(workingPort);
                    labelState.setText("Соединение установлено");
                } else {
                    JOptionPane.showMessageDialog(null, "Соединение уже установлено.");
                }
            } catch (SerialPortException | NullPointerException ex) {
                JOptionPane.showMessageDialog(null, "Ошибка подключения к устройству.");
                portConnection = null;
            }
        } // Конец метода actionPerformed
    } // Конец класса OpenConnectionListener


    /**
     * Класс для прослушивания кнопки "Отключиться"
     */
    private class CloseConnectionListener implements ActionListener {
        @Override
        public void actionPerformed(ActionEvent e) {
            try {

                if (portConnection == null) {
                    JOptionPane.showMessageDialog(null, "Невозможно закрыть соединение, которого нет.");
                    return;
                }

                portConnection.closePortConnection();
                portConnection = null;
                labelState.setText("Соединение отключено.");
                labelState.repaint();
            } catch (SerialPortException ex) {
                JOptionPane.showMessageDialog(null, "Ошибка отключения устройства. " +
                        "Рекомендую выдернуть и всунуть проводок.");
                portConnection = null;
            }
        } // Конец метода actionPerformed
    } // Конец класса CloseConnectionListener


    private class StartButtonListener extends Thread implements ActionListener {
        private boolean isFirstStart = true;

        @Override
        public void actionPerformed(ActionEvent e) {

            if (portConnection != null) {
                isStart = true;
                if (isFirstStart) {
                    this.start();
                    isFirstStart = false;
                }

                try {
                    portConnection.writeByte((byte) 228);
                    portConnection.closePortConnection();
                    portConnection = new PortConnection(workingPort);
                } catch (SerialPortException e1) {
                    e1.printStackTrace();
                }

                labelState.setText("Программа находится в режиме построения графика.");
                labelState.repaint();
            } else {
                JOptionPane.showMessageDialog(null, "Вначале необходимо произвести подключение.");
            }
        } // Конец метода actionPerformed

        @Override
        public void run() {
            counter = 0;
            int distance = 0;
            while (true) {
                try {
                    Thread.sleep(100);
                    if (isStart && counter < 210) {

                        channel1.add((int) (counter++ * (180.0/200)), portConnection.getData()/4);
                    }
                } catch (InterruptedException | NullPointerException e1) {
                    JOptionPane.showMessageDialog(null, ":( Неудача при старте. Возможно стоит проверить соединение.");
                    isStart = false;
                }
                
            }
        } // Конец метода run
    } // Конец класса StartButtonListener



    private class StopButtonListener implements ActionListener {
        @Override
        public void actionPerformed(ActionEvent e) {
            isStart = false;

            try {
                portConnection.writeByte((byte) 228);
                portConnection.closePortConnection();
                portConnection = new PortConnection(workingPort);
            } catch (SerialPortException e1) {
                e1.printStackTrace();
            }

            if (portConnection == null) {
                labelState.setText("График не строится. Соединение отсутствует.");
            } else {
                labelState.setText("График не строится. Соединение не разорвано.");
            }

            labelState.repaint();
        } // Конец метода actionPerformed
    } // Конец класса StopButtonListener

    private class ClearButtonListener implements ActionListener {
        @Override
        public void actionPerformed(ActionEvent event) {
            channel1.clear();
            chartPanel.repaint();
            counter = 0;
        }
    }


    private class SelectPortsListener implements ActionListener {
        String portSelected;

        SelectPortsListener(String portSelected) {
            this.portSelected = portSelected;
        }

        @Override
        public void actionPerformed(ActionEvent event) {
            workingPort = portSelected;
            labelState.setText("Com-порт был выбран.");
            labelState.repaint();
        }
    }


    private class RefreshPortsListener implements ActionListener {
        @Override
        public void actionPerformed(ActionEvent event) {
            portMenu.removeAll();
            String[] portsName = PortConnection.getPorts();
            HashMap<String, JMenuItem> ports = new HashMap<>();

            for (int i = 0; i < portsName.length; ++i) {
                ports.put(portsName[i], new JMenuItem(portsName[i]));
                portMenu.add(ports.get(portsName[i]));
                ports.get(portsName[i]).addActionListener(new SelectPortsListener(portsName[i]));
            }

            JMenuItem refreshPorts = new JMenuItem("Обновить список com-портов.");
            portMenu.add(refreshPorts);
            refreshPorts.addActionListener(new RefreshPortsListener());
            portMenu.repaint();

            labelState.setText("Список com-портов был успешно обновлен.");
            labelState.repaint();
        }
    }

    private class SetAngleButtonListener implements ActionListener {
        @Override
        public void actionPerformed(ActionEvent e) {
            try {
                portConnection.writeByte((byte) 0xFF);
                try {
                    Thread.sleep(1);
                } catch (InterruptedException e1) {
                    e1.printStackTrace();
                }
                portConnection.writeByte((byte) 0x04);
            } catch (SerialPortException | NullPointerException e1) {
                labelState.setText("Ошибка! Возможно вы не подключились к устройству!");
                labelState.repaint();
            }
        }
    }
}
