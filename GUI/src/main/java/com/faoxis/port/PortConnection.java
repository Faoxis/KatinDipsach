package com.faoxis.port;

import com.faoxis.calculation.Coordinate;
import jssc.*;

/**
 * Created by Sergei on 25.04.2016.
 */
public class PortConnection {

    private SerialPort serialPort;
    private int[] dataFromPort = {0, 0};

    public void writeByte(byte data) throws SerialPortException {
        serialPort.writeByte((byte) data);
    }

    //------------------------------------- Конструктор с одним параметром ---------------------------------------//
    public PortConnection(String port) throws SerialPortException {
        this(port, SerialPort.BAUDRATE_115200);
    } // Конец конструктора с единственным параметром
    //------------------------------------------------------------------------------------------------------------//


    //------------------------------------- Конструктор с baudrate ----------------------------------------------//
    public PortConnection(String port, int baudrate) throws SerialPortException {
        this(port, baudrate, SerialPort.DATABITS_8, SerialPort.STOPBITS_1, SerialPort.PARITY_NONE);
    } // Конец конструктора с baudrate
    //-----------------------------------------------------------------------------------------------------------//

    //-------------------------------------- Конструктор с параметрами ------------------------------------------//
    public PortConnection(String port, int baudrate, int databits, int stopbits, int parity) throws SerialPortException {
        serialPort = new SerialPort(port);
        //Открываем порт
        serialPort.openPort();
        serialPort.setParams(baudrate, databits, stopbits, parity);

        // Включаем аппаратное управление потоком
        serialPort.setFlowControlMode(SerialPort.FLOWCONTROL_RTSCTS_IN |
                SerialPort.FLOWCONTROL_RTSCTS_OUT);

        // Устанавливаем ивент лисенер и маску
        serialPort.addEventListener(new PortReader(), SerialPort.MASK_RXCHAR);

    } // Конец конструктора со всеми параметрами
    //------------------------------------------------------------------------------------------------------------//


    //---------------------------------------- Получение данных с COM-порта --------------------------------------//
    public int getData() {
        return dataFromPort[0];
    } // Конец метода getData()
    //------------------------------------------------------------------------------------------------------------//

    //---------------------------------------- Закрытие порта ----------------------------------------------------//
    public void closePortConnection() throws SerialPortException {
        serialPort.closePort();
    }
    //------------------------------------------------------------------------------------------------------------//

    public void openPortConnection() throws SerialPortException {
        serialPort.openPort();
    }

    //---------------------------------- Статический метод с информацией о портах --------------------------------//
    public static String[] getPorts() {
        return SerialPortList.getPortNames();
    } // Конец метода getPorts()
    //------------------------------------------------------------------------------------------------------------//


    //----------------------------------- Класс - слушатель входящих бит -----------------------------------------//
    private class PortReader implements SerialPortEventListener {
        @Override
        public void serialEvent(SerialPortEvent serialPortEvent) {
            if (serialPortEvent.isRXCHAR() && serialPortEvent.getEventValue() > 0) {
                try {
                    //Получаем ответ от устройства, обрабатываем данные и т.д.
                    dataFromPort = serialPort.readIntArray(1);
                } catch (SerialPortException ex) {
                    System.out.println(ex);
                }
            }
        } // Конец функции serialEvent
    } // Конец класса PortReader
    //------------------------------------------------------------------------------------------------------------//


}
