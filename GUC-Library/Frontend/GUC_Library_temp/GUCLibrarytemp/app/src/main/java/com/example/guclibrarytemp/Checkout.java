package com.example.guclibrarytemp;

import androidx.appcompat.app.AppCompatActivity;

import android.content.Intent;
import android.os.Bundle;
import android.view.View;
import android.widget.ArrayAdapter;
import android.widget.Button;
import android.widget.ListView;
import android.widget.Spinner;
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
import java.util.ArrayList;

public class Checkout extends AppCompatActivity {

    String HostORip = "";

    private RequestQueue requestQueue;
    ArrayList<String> itemsname;
    ArrayList<String> quantities;
    String username;
    Boolean admin;
    double totalprice;

    Spinner sp ;
    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        Intent i = getIntent();
        HostORip = i.getStringExtra("HostOrIp");
        username = i.getStringExtra("Username");
        admin = i.getBooleanExtra("isAdmin", true);
        if (admin != null && !admin && username != null && !username.isEmpty())
        {
            setContentView(R.layout.activity_checkout);

            Intent currenti = getIntent();
            itemsname = currenti.getStringArrayListExtra("itemsname");
            quantities = currenti.getStringArrayListExtra("itemsquantity");
            totalprice = currenti.getDoubleExtra("total price", 0.0);


            ArrayAdapter adptnames = new ArrayAdapter(Checkout.this,
                    android.R.layout.simple_expandable_list_item_1, itemsname);
            ListView lvnames = findViewById(R.id.namelistview);
            lvnames.setAdapter(adptnames);

            ArrayAdapter adptquants = new ArrayAdapter(Checkout.this,
                    android.R.layout.simple_expandable_list_item_1, quantities);
            ListView lvquants = findViewById(R.id.quantitylistview);
            lvquants.setAdapter(adptquants);

            String[] places = {"b1", "c1", "d1"};
            sp = findViewById(R.id.spinnerplace);
            ArrayAdapter adptplaces = new ArrayAdapter(Checkout.this,
                    android.R.layout.simple_expandable_list_item_1, places);
            sp.setAdapter(adptplaces);

            TextView tvtotalprice = findViewById(R.id.totalprice);
            tvtotalprice.setText("Your Bill is: " + totalprice + " LE");


            Button confirm = findViewById(R.id.confirm);
            confirm.setOnClickListener(new View.OnClickListener() {
                @Override
                public void onClick(View v) {
                    String data = "{\n\"items\":[";
                    for (int i = 0; i < itemsname.size(); i++) {
                        data += "{" +
                                "\"itemName\": " + "\"" + itemsname.get(i) + "\"," +
                                "\"quantity\": " + Integer.parseInt(quantities.get(i)) +
                                "}";
                        if (!(i + 1 == itemsname.size()))
                            data += ",";
                    }
                    data += "],\n";


                    data += "\"userName\": " + "\"" + username + "\",\n" +
                            "\"place\": " + "\"" + sp.getSelectedItem().toString() + "\",\n" +
                            "\"totalPrice\": " + totalprice +
                            "\n}";
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
        final String savedata = data;
        String URL = HostORip + "/Purchase/newPurchase";

        requestQueue = Volley.newRequestQueue(getApplicationContext());
        StringRequest stringRequest = new StringRequest(Request.Method.POST, URL, new Response.Listener<String>() {
            @Override
            public void onResponse(String response) {
                try {
                    JSONObject objres=new JSONObject(response);
                    Toast.makeText(getApplicationContext(),"Purchase Done",Toast.LENGTH_LONG).show();


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
