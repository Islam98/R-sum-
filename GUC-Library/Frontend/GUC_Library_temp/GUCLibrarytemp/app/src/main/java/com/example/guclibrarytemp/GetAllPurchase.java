package com.example.guclibrarytemp;

import androidx.appcompat.app.AppCompatActivity;

import android.content.Intent;
import android.os.Bundle;
import android.view.View;
import android.widget.AdapterView;
import android.widget.ArrayAdapter;
import android.widget.ListView;

import com.android.volley.Request;
import com.android.volley.RequestQueue;
import com.android.volley.Response;
import com.android.volley.VolleyError;
import com.android.volley.toolbox.JsonArrayRequest;
import com.android.volley.toolbox.Volley;

import org.json.JSONArray;
import org.json.JSONException;
import org.json.JSONObject;

import java.util.ArrayList;

public class GetAllPurchase extends AppCompatActivity {

    String HostORip = "";

    ArrayAdapter atnames;
    private RequestQueue mQueue;

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
            setContentView(R.layout.activity_get_all_purchase);

            final ArrayList<String> alltobeshown = new ArrayList<String>();

            mQueue = Volley.newRequestQueue(this);
            String url = HostORip+"/Purchase/getAllPurchase";
            JsonArrayRequest jsonreq = new JsonArrayRequest(Request.Method.POST, url, null,
                    new Response.Listener<JSONArray>() {
                        @Override
                        public void onResponse(JSONArray response) {
                            try {
                                for (int i = 0; i < response.length(); i++) {
                                    JSONObject item = response.getJSONObject(i);

                                    JSONArray purchases = item.getJSONArray("items");
                                    String itemss = "items: ";
                                    for (int j = 0; j < purchases.length(); j++) {
                                        itemss += "item" + j + (purchases.getJSONObject(j)).toString();
                                        if (j + 1 != purchases.length())
                                            itemss += ", ";
                                    }
                                    String username = "username: " + item.getString("userName");
                                    String check = "check: " + item.getBoolean("paidCheck");
                                    String date = "date: " + item.getString("date");
                                    String totalprice = "totalprice: " + item.getDouble("totalPrice");
                                    String place = "place: " + item.getString("place");

                                    String finalstring = username + ". " + itemss + ".  " + check + ".  " +
                                            date + ".  " + totalprice + ".  " + place + ".";

                                    alltobeshown.add(finalstring);
                                }
                                atnames = new ArrayAdapter(GetAllPurchase.this,
                                        android.R.layout.simple_expandable_list_item_1, alltobeshown);

                                ListView lvpurch = findViewById(R.id.Viewpurchase);
                                lvpurch.setAdapter(atnames);
                                lvpurch.setOnItemClickListener(new AdapterView.OnItemClickListener() {
                                    @Override
                                    public void onItemClick(AdapterView<?> parent, View view, int position, long id) {
                                        Intent i = new Intent(getApplicationContext(), viewpurchase.class);
                                        i.putExtra("Username", username);
                                        i.putExtra("isAdmin", admin);
                                        i.putExtra("data", alltobeshown.get((int) id));
                                        i.putExtra("HostOrIp", HostORip);
                                        startActivity(i);
                                    }
                                });
                            } catch (JSONException e) {
                                e.printStackTrace();
                            }
                        }
                    }, new Response.ErrorListener() {
                @Override
                public void onErrorResponse(VolleyError error) {
                    error.printStackTrace();
                }
            });

            mQueue.add(jsonreq);
        }
        else {
            Intent nextintent = new Intent(getApplicationContext(), errorpage.class);
            startActivity(nextintent);
        }
    }
}
