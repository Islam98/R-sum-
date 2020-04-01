package com.example.guclibrarytemp;

import androidx.appcompat.app.AppCompatActivity;

import android.os.Bundle;

public class errorpage extends AppCompatActivity {

    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        setContentView(R.layout.activity_errorpage);

        ErrorDialog error = new ErrorDialog("     You cannot access this page");
        error.show(getSupportFragmentManager(), "Error");
    }
}
