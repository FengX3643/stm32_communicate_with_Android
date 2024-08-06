package com.example.myapplication;

import android.annotation.SuppressLint;
import android.app.Activity;
import android.content.Intent;
import android.content.SharedPreferences;
import android.os.Build;
import android.os.Bundle;
import android.util.Log;
import android.view.View;
import android.widget.Button;
import android.widget.SeekBar;
import android.widget.TextView;

import androidx.appcompat.app.AppCompatActivity;

public class activity_2 extends AppCompatActivity implements View.OnClickListener {
    private TextView textView_1;
    private TextView textView_2;
    private TextView textView_3;
    private TextView textView_4;
    private SeekBar seekBar_1;
    private SeekBar seekBar_2;
    private SeekBar seekBar_3;
    private SeekBar seekBar_4;
    private String str_1 = "10";
    private String str_2 = "16";
    private String str_3 = "16";
    private String str_4 = "60";
    private SharedPreferences.Editor editor;


    @SuppressLint({"SetTextI18n", "DefaultLocale"})
    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        setContentView(R.layout.activity_2);

        editor = getSharedPreferences("test", MODE_PRIVATE).edit();
        SharedPreferences spf = getSharedPreferences("test", MODE_PRIVATE);

        str_1 = spf.getString("1", str_1);
        str_2 = spf.getString("2", str_2);
        str_3 = spf.getString("3", str_3);
        str_4 = spf.getString("4", str_4);

        textView_1 = findViewById(R.id.textView_1);
        textView_2 = findViewById(R.id.textView_2);
        textView_3 = findViewById(R.id.textView_3);
        textView_4 = findViewById(R.id.textView_4);

        textView_1.setText("闹钟持续时间:" + str_1 + "分钟");
        textView_2.setText(String.format("闹钟电压:%.1fV", Integer.parseInt(str_2) / 2.0 / 10.0));
        textView_3.setText(String.format("床灯电压%.1fV", Integer.parseInt(str_3) / 2.0 / 10.0));
        textView_4.setText(String.format("柜灯电压:%.1fV", Integer.parseInt(str_4) / 2.0 / 10.0));
        seekBar_1 = findViewById(R.id.seekBar_1);
        seekBar_2 = findViewById(R.id.seekBar_2);
        seekBar_3 = findViewById(R.id.seekBar_3);
        seekBar_4 = findViewById(R.id.seekBar_4);
        seekBar_1.setMax(60);
        if (Build.VERSION.SDK_INT >= Build.VERSION_CODES.O) {
            seekBar_1.setMin(1);
        }
        seekBar_2.setMax(99);
        if (Build.VERSION.SDK_INT >= Build.VERSION_CODES.O) {
            seekBar_2.setMin(1);
        }
        seekBar_3.setMax(99);
        if (Build.VERSION.SDK_INT >= Build.VERSION_CODES.O) {
            seekBar_3.setMin(1);
        }
        seekBar_4.setMax(70);
        if (Build.VERSION.SDK_INT >= Build.VERSION_CODES.O) {
            seekBar_1.setMin(1);
        }


        seekBar_1.setProgress(Integer.parseInt(str_1));

        seekBar_2.setProgress(Integer.parseInt(str_2));

        seekBar_3.setProgress(Integer.parseInt(str_3));

        seekBar_4.setProgress(Integer.parseInt(str_4));

        Button button6 = findViewById(R.id.button6);
        button6.setOnClickListener(this);

        SeekBar.OnSeekBarChangeListener seekBarChangeListener = new SeekBar.OnSeekBarChangeListener() {
            @SuppressLint({"SetTextI18n", "DefaultLocale"})
            @Override
            public void onProgressChanged(SeekBar seekBar, int i, boolean b) {
                //
                if (seekBar == seekBar_1) {
                    str_1 = String.format("%02d", i);
                    textView_1.setText(String.format("闹钟持续时间:%02d分钟", i));
                    editor.putString("1", str_1);

                }
                if (seekBar == seekBar_2) {
                    str_2 = String.format("%02d", i);
                    textView_2.setText(String.format("闹钟电压:%.1fV", i / 2.0 / 10.0));
                    editor.putString("2", str_2);

                }
                if (seekBar == seekBar_3) {
                    str_3 = String.format("%02d", i);
                    textView_3.setText(String.format("床灯电压%.1fV", i / 2.0 / 10.0));
                    editor.putString("3", str_3);

                }
                if (seekBar == seekBar_4) {
                    str_4 = String.format("%02d", i);
                    textView_4.setText(String.format("柜灯电压:%.1fV", i / 2.0 / 10.0));
                    editor.putString("4", str_4);

                }
            }

            @Override
            public void onStartTrackingTouch(SeekBar seekBar) {

            }

            @Override
            public void onStopTrackingTouch(SeekBar seekBar) {

            }
        };
        seekBar_1.setOnSeekBarChangeListener(seekBarChangeListener);
        seekBar_2.setOnSeekBarChangeListener(seekBarChangeListener);
        seekBar_3.setOnSeekBarChangeListener(seekBarChangeListener);
        seekBar_4.setOnSeekBarChangeListener(seekBarChangeListener);

    }


    @Override
    public void onClick(View view) {
        if (view.getId() == R.id.button6) {
            editor.apply();
            Intent intent = new Intent();
            Bundle bundle = new Bundle();
            bundle.putString("set_code", "S" + str_1 + str_2 + str_3 + str_4);
            Log.e("Set_code","S" + str_1 + str_2 + str_3 + str_4);
            intent.putExtras(bundle);
            // 携带意图返回上一个页面 RESULT_OK代表处理成功了
            setResult(Activity.RESULT_OK, intent);
            // 结束当前页面
            finish();
        }
    }

}