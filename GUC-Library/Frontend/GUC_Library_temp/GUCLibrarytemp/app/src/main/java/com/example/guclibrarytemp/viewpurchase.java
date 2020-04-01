package com.example.guclibrarytemp;

import androidx.appcompat.app.AppCompatActivity;

import android.content.Intent;
import android.os.Bundle;
import android.widget.TextView;

public class viewpurchase extends AppCompatActivity {


    String username;
    boolean admin;
    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        Intent currentintent = getIntent();
        username = currentintent.getStringExtra("Username");
        admin = currentintent.getBooleanExtra("isAdmin", false);

        if (admin && (username != null || !username.equals(""))) {
            setContentView(R.layout.activity_viewpurchase);

            Intent intent = getIntent();
            String purchasedata = intent.getStringExtra("data");
            TextView tvpurchase = findViewById(R.id.purchaseview);
            tvpurchase.setText(purchasedata);
        }
        else {
            Intent nextintent = new Intent(getApplicationContext(), errorpage.class);
            startActivity(nextintent);
        }
    }
}
