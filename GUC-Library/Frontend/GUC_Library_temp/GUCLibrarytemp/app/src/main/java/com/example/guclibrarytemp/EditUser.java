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

import org.json.JSONArray;
import org.json.JSONException;

import java.io.UnsupportedEncodingException;
import java.util.ArrayList;

//post request then with updated user data
//update user attributes using body then send post request
public class EditUser extends AppCompatActivity {

    String HostORip = "";

    private RequestQueue requestQueue;
    EditText etun;

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
            setContentView(R.layout.activity_edit_user);

            etun = findViewById(R.id.Username);
            Button bgo = findViewById(R.id.EditCheck);
            bgo.setOnClickListener(new View.OnClickListener() {
                @Override
                public void onClick(View v) {
                    String un = etun.getText().toString();
                    String data = "{" +
                            "\"userName\":" + "\"" + un + "\"" +
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
        String URL = HostORip + "/Purchase/getPurchaseForUser";

        requestQueue = Volley.newRequestQueue(getApplicationContext());
        StringRequest stringRequest = new StringRequest(Request.Method.POST, URL, new Response.Listener<String>() {
            @Override
            public void onResponse(String response) {
                try {
 //                   System.out.println(response + "================================================================================================================================");
                    JSONArray objres = new JSONArray(response);
                    JSONArray purchases = objres;
          //          objres.toJSONArray(purchases);

                    ArrayList<String> date = new ArrayList<String>();
                    ArrayList<String> check = new ArrayList<String>();
                    ArrayList<String> totalprice = new ArrayList<String>();
                    ArrayList<String> allitems = new ArrayList<String>();

                    for(int i = 0 ; i < purchases.length(); i++)
                    {
                        date.add(purchases.getJSONObject(i).getString("date"));
                        check.add(purchases.getJSONObject(i).getBoolean("paidCheck")+"");
                        totalprice.add(purchases.getJSONObject(i).getDouble("totalPrice")+"");

                        JSONArray JAitems = purchases.getJSONObject(i).getJSONArray("items");
                        String s = "";
                        for(int j = 0 ; j < JAitems.length() ; j++)
                        {
                            int indextemp = j + 1;
                //            System.out.println(JAitems + "================================================================================================================================");

                            s += "Item" + indextemp + "Name: " + JAitems.getJSONObject(j).getString("itemName") +
                                    ", Quantity: " + JAitems.getJSONObject(j).getInt("quantity") + ". ";
                        }
               //         System.out.println("done here222  " + "================================================================================================================================");

                        allitems.add(s);
                    }
                    Intent i = new Intent(getApplicationContext(), check.class);
                    i.putStringArrayListExtra("date", date);
                    i.putStringArrayListExtra("check", check);
                    i.putStringArrayListExtra("totalp", totalprice);
                    i.putStringArrayListExtra("item", allitems);
                    i.putExtra("Username", username);
                    i.putExtra("isAdmin", admin);
                    i.putExtra("HostOrIp", HostORip);
                    startActivity(i);

                } catch (JSONException e) {
                    Toast.makeText(getApplicationContext(), "Server Error 101", Toast.LENGTH_LONG).show();

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


