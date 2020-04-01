package com.example.guclibrarytemp;

import androidx.appcompat.app.AppCompatActivity;

import android.content.Intent;
import android.os.Bundle;
import android.view.View;
import android.widget.AdapterView;
import android.widget.ArrayAdapter;
import android.widget.ListView;
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

public class check extends AppCompatActivity {

    String HostORip = "";

    private RequestQueue requestQueue;
    ArrayList<String> date;
    ArrayList<String> check;
    ArrayList<String> totalprice;
    ArrayList<String> item;

    String username;
    boolean admin;
    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);

        Intent curerentintent = getIntent();
        HostORip = curerentintent.getStringExtra("HostOrIp");
        username = curerentintent.getStringExtra("Username");
        admin = curerentintent.getBooleanExtra("isAdmin", false);

        if (admin && (username != null || !username.equals("")))
        {
            setContentView(R.layout.activity_check);

            Intent intent = getIntent();
            date = intent.getStringArrayListExtra("date");
            check = intent.getStringArrayListExtra("check");
            totalprice = intent.getStringArrayListExtra("totalp");
            item = intent.getStringArrayListExtra("item");

            ArrayList<String> showdata = new ArrayList<String>();

            for (int i = 0; i < date.size(); i++) {
                showdata.add("Date: " + date.get(i) + ". Items: " + item.get(i) + ". TotalPrice: " +
                        totalprice.get(i) + ". Check: " + check.get(i));
            }

            //   System.out.println(date.size()+"++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++");

            ListView lvuser = findViewById(R.id.Listviewpurchases);

            ArrayAdapter atusers = new ArrayAdapter(check.this,
                    android.R.layout.simple_expandable_list_item_1, showdata);

            lvuser.setAdapter(atusers);

            lvuser.setOnItemClickListener(new AdapterView.OnItemClickListener() {
                @Override
                public void onItemClick(AdapterView<?> parent, View view, int position, long id) {
                    String datechoosed = date.get((int) id);


                    String data = "{" +
                            "\"date\":" + "\"" + datechoosed + "\"" +
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

    private void Submit(String data) {
        final String savedata = data;
        String URL = HostORip + "/Purchase/paid";

        requestQueue = Volley.newRequestQueue(getApplicationContext());
        StringRequest stringRequest = new StringRequest(Request.Method.POST, URL, new Response.Listener<String>() {
            @Override
            public void onResponse(String response) {
                try {
                    JSONObject objres = new JSONObject(response);
                    if(!response.contains("WRONG"))
                        Toast.makeText(getApplicationContext(),"done successfully",Toast.LENGTH_LONG).show();
                    else
                    Toast.makeText(getApplicationContext(),"ERROR",Toast.LENGTH_LONG).show();
//                    System.out.println(objres);
                } catch (JSONException e) {
                    Toast.makeText(getApplicationContext(), "Server Error", Toast.LENGTH_LONG).show();

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
