package com.example.robo;

import androidx.annotation.Nullable;
import androidx.appcompat.app.AppCompatActivity;

import android.app.Activity;
import android.app.AlertDialog;
import android.bluetooth.BluetoothAdapter;
import android.content.DialogInterface;
import android.content.Intent;
import android.os.Bundle;
import android.view.View;
import android.widget.Button;
import android.widget.Toast;

public class AutoActivity extends AppCompatActivity {

    private AlertDialog.Builder dialog;
    private Button btn_manual, btn_conexao;
    private BluetoothAdapter meuBluetoothAdapter = null;
    private static final int SOLICITA_ATIVACAO = 1;
    private boolean conexao = false;

    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        setContentView(R.layout.activity_auto);
        btn_manual = findViewById(R.id.button);
        btn_conexao = findViewById(R.id.conexao_id);

        meuBluetoothAdapter = BluetoothAdapter.getDefaultAdapter();
        if(meuBluetoothAdapter == null){
            Toast.makeText(getApplicationContext(),"O dispositivo não suporta a aplicação!", Toast.LENGTH_LONG).show();
            finishAffinity();
        }
        else if(!meuBluetoothAdapter.isEnabled()){
            Intent ativaBluetooth = new Intent(BluetoothAdapter.ACTION_REQUEST_ENABLE);
            startActivityForResult(ativaBluetooth, SOLICITA_ATIVACAO);
        }


        btn_conexao.setOnClickListener(new View.OnClickListener() {
            @Override
            public void onClick(View v) {
                if(conexao){
                    //desconectar
                }
                else{
                    //Conectar
                }
            }
        });
    }

    @Override
    protected void onActivityResult(int requestCode, int resultCode, @Nullable Intent data) {
        switch (requestCode){
            case SOLICITA_ATIVACAO:
                if(resultCode == Activity.RESULT_OK){
                    Toast.makeText(getApplicationContext(),"Bluetooth ativado!", Toast.LENGTH_LONG).show();
                }
                else{
                    Toast.makeText(getApplicationContext(),"Bluetooth não ativado! O app será encerrado", Toast.LENGTH_LONG).show();
                    finishAffinity();
                }
                break;
        }
    }

    public void onBackPressed(){

        dialog = new AlertDialog.Builder(AutoActivity.this);

        dialog.setMessage("Deseja sair?");

        dialog.setNegativeButton("Não", new DialogInterface.OnClickListener() {
            @Override
            public void onClick(DialogInterface dialog, int which) {
                return;
            }
        });
        dialog.setPositiveButton("Sim", new DialogInterface.OnClickListener() {
            @Override
            public void onClick(DialogInterface dialog, int which) {
                finishAffinity();
            }
        });

        dialog.create();
        dialog.show();
    }
            }
