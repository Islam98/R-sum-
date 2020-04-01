package com.example.guclibrarytemp;

import androidx.appcompat.app.AppCompatActivity;

import android.content.Intent;
import android.os.Bundle;
import android.view.View;
import android.widget.Button;
import android.widget.EditText;
import android.widget.TextView;
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

public class quantityeditor extends AppCompatActivity {

    String HostORip = "";

    private RequestQueue requestQueue;

    String username;
    boolean admin;
    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);

        Intent currentintent = getIntent();
        HostORip = currentintent.getStringExtra("HostOrIp");
        username = currentintent.getStringExtra("Username");
        admin = currentintent.getBooleanExtra("isAdmin", false);

        if (admin && (username != null || !username.equals("")))
        {
            setContentView(R.layout.activity_quantityeditor);
            Intent i = getIntent();
            final String itemname = i.getStringExtra("itemname");
            String itemdesc = i.getStringExtra("itemdesc");
            int itemquantity = i.getIntExtra("itemquantity", 0);
            double itemprice = i.getDoubleExtra("itemprice", 0.0);

            TextView tvname = findViewById(R.id.itemname);
            TextView tvdesc = findViewById(R.id.itemdesc);
            TextView tvprice = findViewById(R.id.itemprice);
            TextView tvquant = findViewById(R.id.itemquantity);
            final EditText quantnew = findViewById(R.id.quantityentered);
            Button sendupdate = findViewById(R.id.sendupdate);

            tvname.setText(itemname);
            tvdesc.setText(itemdesc);
            tvprice.setText(itemprice + "");
            tvquant.setText("current quantity is: " + itemquantity);

            sendupdate.setOnClickListener(new View.OnClickListener() {
                @Override
                public void onClick(View v) {
                    int quantity = Integer.parseInt(quantnew.getText().toString());

                    String data = "{" +
                            "\"name\":" + "\"" + itemname + "\"," +
                            "\"quantity\":" + " " + quantity + " " +
                            "\n }";
                    Submit(data);
                }
            });
        }
        else
        {
            Intent nextintent = new Intent(getApplicationContext(), errorpage.class);
            startActivity(nextintent);
        }
    }

    private void Submit(String data)
    {
        final String savedata= data;
        String URL=HostORip+"/Items/updateQuantity";

        requestQueue = Volley.newRequestQueue(getApplicationContext());
        StringRequest stringRequest = new StringRequest(Request.Method.POST, URL, new Response.Listener<String>() {
            @Override
            public void onResponse(String response) {
                try {
                    JSONObject objres=new JSONObject(response);
                    Toast.makeText(getApplicationContext(),"item updated successfully",Toast.LENGTH_LONG).show();


                } catch (JSONException e) {
                    Toast.makeText(getApplicationContext(),"Server Error",Toast.LENGTH_LONG).show();

                }
                //Log.i("VOLLEY", response);
            }
        }, new Response.ErrorListener() {
            @Override
            public void onErrorResponse(VolleyError error) {

                Toast.makeText(getApplicationContext(), error.getMessage(), Toast.LENGTH_SHORT).show();

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
