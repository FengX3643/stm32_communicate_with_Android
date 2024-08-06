package com.example.myapplication;

import android.annotation.SuppressLint;
import android.app.Activity;
import android.bluetooth.BluetoothGatt;
import android.content.Intent;
import android.os.Build;
import android.os.Bundle;
import android.view.View;
import android.widget.Button;
import android.widget.CompoundButton;
import android.widget.Switch;
import android.widget.TextView;
import android.widget.TimePicker;

import androidx.activity.result.ActivityResultLauncher;
import androidx.activity.result.contract.ActivityResultContracts;
import androidx.annotation.NonNull;
import androidx.appcompat.app.AppCompatActivity;

import com.clj.fastble.BleManager;
import com.clj.fastble.callback.BleScanCallback;
import com.clj.fastble.callback.BleWriteCallback;
import com.clj.fastble.data.BleDevice;
import com.clj.fastble.exception.BleException;
import com.clj.fastble.scan.BleScanRuleConfig;

import java.util.List;

@SuppressLint("UseSwitchCompatOrMaterialCode")
public class activity_main extends AppCompatActivity implements View.OnClickListener {
    public TextView tv_1;
    public TextView tv_2;
    public TextView tv_3;
    public TextView tv_4;
    public TextView tv_5;

    public Switch switch1;
    public Switch switch2;
    public Switch switch3;
    public TimePicker pick;
    public String names = "BT24";
    public String Mac = "48:87:2D:9B:32:7E";
    public String uuid_service = "0000ffe0-0000-1000-8000-00805f9b34fb";
    public String uuid_characteristic_notify = "0000ffe1-0000-1000-8000-00805f9b34fb";
    public Button bt_1;

    public BleDevice Need_bledevice;
    public BluetoothGatt bluetoothGatt;
    private ActivityResultLauncher<Intent> register;


    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        setContentView(R.layout.activity_main);
        register = registerForActivityResult(new ActivityResultContracts.StartActivityForResult(), result -> {

            if (result != null) {
                Intent intent = result.getData();
                if (intent != null && result.getResultCode() == Activity.RESULT_OK) {
                    // 下一个Activity回退后 在此获取返回的值
                    Bundle myBundle = intent.getExtras();
                    if (myBundle != null) {
                        String set_code = myBundle.getString("set_code");
                        writeToBleDevice(set_code);
                        tv_2.setText(set_code);
                    }
                }
            }
        });
        //实例化控件
        tv_1 = findViewById(R.id.tv_1);
        tv_2 = findViewById(R.id.tv_2);
        tv_3 = findViewById(R.id.tv_3);
        tv_4 = findViewById(R.id.tv_4);
        tv_5 = findViewById(R.id.tv_5);


        pick = findViewById(R.id.pick);
        switch1 = findViewById(R.id.switch1);
        switch2 = findViewById(R.id.switch2);
        switch3 = findViewById(R.id.switch3);

        bt_1 = findViewById(R.id.bt_1);
        Button bt_2 = findViewById(R.id.bt_2);
        bt_1.setOnClickListener(this);
        bt_2.setOnClickListener(this);
        switch1.setOnCheckedChangeListener(new CompoundButton.OnCheckedChangeListener() {
            @Override
            public void onCheckedChanged(CompoundButton compoundButton, boolean b) {
                if (b) {
                    writeToBleDevice("N_on");
                } else {
                    writeToBleDevice("N_off");
                }
            }
        });
        switch3.setOnCheckedChangeListener(new CompoundButton.OnCheckedChangeListener() {
            @Override
            public void onCheckedChanged(CompoundButton compoundButton, boolean b) {
                if (b) {
                    writeToBleDevice("2_on");
                } else {
                    writeToBleDevice("2_off");
                }
            }
        });
        switch2.setOnCheckedChangeListener(new CompoundButton.OnCheckedChangeListener() {
            @Override
            public void onCheckedChanged(CompoundButton compoundButton, boolean b) {
                if (b) {
                    writeToBleDevice("1_on");
                } else {
                    writeToBleDevice("1_off");
                }
            }
        });
//获取蓝牙权限
        BleManager.getInstance().init(getApplication());
        BleManager.getInstance()
                .enableLog(true)
                .setReConnectCount(1, 5000)
                .setOperateTimeout(5000);

        BleManager.getInstance().isBlueEnable();
        BleManager.getInstance().enableBluetooth();
//  打开蓝牙
        BleScanRuleConfig scanRuleConfig = new BleScanRuleConfig.Builder()
                .setDeviceMac(Mac)                  // 只扫描指定mac的设备，可选
                .setDeviceName(true, names)         // 只扫描指定广播名的设备，可选
                .setScanTimeOut(10000)              // 扫描超时时间，可选，默认10秒
                .build();
        BleManager.getInstance().initScanRule(scanRuleConfig);
        BleManager.getInstance().scan(new BleScanCallback() {
            @Override
            public void onScanStarted(boolean success) {

            }

            @Override
            public void onLeScan(BleDevice bleDevice) {

            }

            @Override
            public void onScanning(BleDevice bleDevice) {
                BleManager.getInstance().cancelScan();
            }

            @Override
            public void onScanFinished(List<BleDevice> scanResultList) {

                Need_bledevice = scanResultList.get(0);
                tv_2.setText(Need_bledevice.getName() + Need_bledevice.getDevice().toString());


            }


        });


    }

    @Override
    public void onClick(View view) {

        if (view.getId() == R.id.bt_1) {
            if (Build.VERSION.SDK_INT >= Build.VERSION_CODES.M) {
                int hour = pick.getHour();
                int minute = pick.getMinute();
                @SuppressLint("DefaultLocale") String clock = String.format("%02d%02d", hour, minute);
                writeToBleDevice("n" + clock);
            }

        }
        if (view.getId() == R.id.bt_2) {

            Intent intent = new Intent(this, activity_2.class);

            // 用register.launch跳转至B
            register.launch(intent);

        }
    }

    public void writeToBleDevice(String a) {
        a = "@" + a + "#";
        BleManager.getInstance().write(
                Need_bledevice,
                uuid_service,
                uuid_characteristic_notify,
                a.getBytes(),
                new BleWriteCallback() {
                    @Override
                    public void onWriteSuccess(int current, int total, byte[] justWrite) {
                        // 发送数据到设备成功（分包发送的情况下，可以通过方法中返回的参数可以查看发送进度）
                    }

                    @Override
                    public void onWriteFailure(BleException exception) {
                        // 发送数据到设备失败
                    }
                });
    }

    public class myThread extends Thread {
        public myThread(@NonNull String name) {
            super(name);
        }

        @Override
        public void run() {
            while (true) {
                try {
                    writeToBleDevice("c");
                    Thread.sleep(2000);
                } catch (InterruptedException e) {
                    throw new RuntimeException(e);
                }

            }
        }
    }


}