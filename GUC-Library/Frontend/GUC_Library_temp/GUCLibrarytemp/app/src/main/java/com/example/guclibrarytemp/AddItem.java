package com.example.guclibrarytemp;

import androidx.appcompat.app.AppCompatActivity;

import android.content.Intent;
import android.os.Bundle;
import android.view.View;
import android.widget.Button;
import android.widget.EditText;
import android.widget.Toast;

import com.android.volley.AuthFailureError;
import com.android.volley.Request;
import com.android.volley.RequestQueue;
import com.android.volley.Response;
import com.android.volley.VolleyError;
import com.android.volley.toolbox.StringRequest;
import com.android.volley.toolbox.Volley;

import org.json.JSONException;
import org.json.JSONObject;

import java.io.UnsupportedEncodingException;

public class AddItem extends AppCompatActivity {

    String HostORip = "";

    String username;
    boolean admin;
    private RequestQueue requestQueue;
    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);


        Intent i = getIntent();
        HostORip = i.getStringExtra("HostOrIp");
        username = i.getStringExtra("Username");
         admin = i.getBooleanExtra("isAdmin", false);

        if (admin && (username != null && !username.equals(""))) {
                setContentView(R.layout.activity_add_item);

            final EditText itemname = findViewById(R.id.ItemName);
            final EditText itemdesc = findViewById(R.id.ItemDesc);
            final EditText itemprice = findViewById(R.id.ItemPrice);
            final EditText itemquan = findViewById(R.id.ItemQuantity);
            Button add = findViewById(R.id.AddItem);

            add.setOnClickListener(new View.OnClickListener() {
                @Override
                public void onClick(View v) {
                    String Name = itemname.getText().toString();
                    String Desc = itemdesc.getText().toString();
                    double Price = Double.parseDouble(itemprice.getText().toString());
                    int Quantity = Integer.parseInt(itemquan.getText().toString());

                    String data = "{" +
                            "\"name\":" + "\"" + Name + "\"," +
                            "\"description\":" + "\"" + Desc + "\"," +
                            "\"price\":" + " " + Price + " ," +
                            "\"quantity\":" + " " + Quantity + " " +
                            "}";
                    Submit(data);
                }
            });
        }
        else {
            Intent nextintent = new Intent(getApplicationContext(), errorpage.class);
            startActivity(nextintent);
        }
    }
    private void Submit(String data)
    {
        final String savedata= data;
        String URL=HostORip+"/Items/newItem";
        requestQueue = Volley.newRequestQueue(getApplicationContext());
        StringRequest stringRequest = new StringRequest(Request.Method.POST,
                URL, new Response.Listener<String>() {
            @Override
            public void onResponse(String response) {
                try {
                    JSONObject jsonres = new JSONObject(response);
                    Toast.makeText(getApplicationContext(),"Item created successfully",Toast.LENGTH_LONG).show();
                } catch (JSONException e) {
                    Toast.makeText(getApplicationContext(),"Server Error",Toast.LENGTH_LONG).show();
                }
            }
        }, new Response.ErrorListener() {
            @Override
            public void onErrorResponse(VolleyError error) {
                Toast.makeText(getApplicationContext(), error.getMessage(),
                        Toast.LENGTH_SHORT).show();
//Log.v("VOLLEY", error.toString());
            }
        }) {
            @Override
            public String getBodyContentType() {
                return "application/json; charset=utf-8";
            }
            @Override
            public byte[] getBody() throws AuthFailureError {
                try {
                    return savedata == null ? null : savedata.getBytes("utf-8");
                } catch (UnsupportedEncodingException uee) {
//Log.v("Unsupported Encoding while trying to get the bytes", data);
                    return null;
                }
            }
        };
        requestQueue.add(stringRequest);
    }
}
