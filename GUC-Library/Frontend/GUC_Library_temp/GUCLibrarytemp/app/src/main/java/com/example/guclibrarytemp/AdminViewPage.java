package com.example.guclibrarytemp;

import androidx.appcompat.app.AppCompatActivity;

import android.content.Intent;
import android.os.Bundle;
import android.view.View;
import android.widget.Button;

public class AdminViewPage extends AppCompatActivity {

    String HostORip = "";
    String username;
    boolean admin;

    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);

        Intent i = getIntent();
        HostORip = i.getStringExtra("HostOrIp");
        username = i.getStringExtra("Username");
        admin = i.getBooleanExtra("isAdmin", false);

        if (admin && (username != null || !username.equals(""))) {
            setContentView(R.layout.activity_admin_view_page);

            Button additem = findViewById(R.id.AddItem);
            additem.setOnClickListener(new View.OnClickListener() {
                @Override
                public void onClick(View v) {
                    Intent i = new Intent(getApplicationContext(), AddItem.class);
                    i.putExtra("Username", username);
                    i.putExtra("isAdmin", admin);
                    i.putExtra("HostOrIp", HostORip);
                    startActivity(i);
                }
            });
            Button updateitems = findViewById(R.id.UpdateItem);
            updateitems.setOnClickListener(new View.OnClickListener() {
                @Override
                public void onClick(View v) {
                    Intent i = new Intent(getApplicationContext(), insertitemname.class);
                    i.putExtra("Username", username);
                    i.putExtra("isAdmin", admin);
                    i.putExtra("HostOrIp", HostORip);
                    startActivity(i);
                }
            });


            Button getpurchases = findViewById(R.id.GetPurchases);
            getpurchases.setOnClickListener(new View.OnClickListener() {
                @Override
                public void onClick(View v) {
                    Intent i = new Intent(getApplicationContext(), GetAllPurchase.class);
                    i.putExtra("Username", username);
                    i.putExtra("isAdmin", admin);
                    i.putExtra("HostOrIp", HostORip);
                    startActivity(i);
                }
            });
            Button updateuserchecklist = findViewById(R.id.updatechecklist);
            updateuserchecklist.setOnClickListener(new View.OnClickListener() {
                @Override
                public void onClick(View v) {
                    Intent i = new Intent(getApplicationContext(), EditUser.class);
                    i.putExtra("Username", username);
                    i.putExtra("isAdmin", admin);
                    i.putExtra("HostOrIp", HostORip);
                    startActivity(i);
                }
            });
        } else {
            Intent nextintent = new Intent(getApplicationContext(), errorpage.class);
            startActivity(nextintent);
        }

    }
}
