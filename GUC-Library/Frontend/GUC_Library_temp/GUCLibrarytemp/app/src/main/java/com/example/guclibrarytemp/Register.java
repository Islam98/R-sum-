package com.example.guclibrarytemp;

import androidx.appcompat.app.AppCompatActivity;

import android.content.Intent;
import android.os.Bundle;
import android.view.View;
import android.widget.ArrayAdapter;
import android.widget.Button;
import android.widget.EditText;
import android.widget.Toast;

import com.android.volley.AuthFailureError;
import com.android.volley.Request;
import com.android.volley.RequestQueue;
import com.android.volley.Response;
import com.android.volley.VolleyError;
import com.android.volley.toolbox.JsonArrayRequest;
import com.android.volley.toolbox.StringRequest;
import com.android.volley.toolbox.Volley;

import org.json.JSONArray;
import org.json.JSONException;
import org.json.JSONObject;

import java.io.UnsupportedEncodingException;
import java.util.ArrayList;

public class Register extends AppCompatActivity {

    String HostORip = "";
    EditText etusername;
    EditText etpassword;
    EditText etmail;
    ArrayList<String> usernames = new ArrayList<String>();
    ArrayList<String> useremails = new ArrayList<String>();

    Button Bsignedup;

    private RequestQueue mQueue;
    private RequestQueue requestQueue;

    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        setContentView(R.layout.activity_register);

        Intent i = getIntent();
        HostORip = i.getStringExtra("HostOrIp");

        etusername = findViewById(R.id.Username);
        etpassword = findViewById(R.id.Password);
        etmail = findViewById(R.id.Email);


        mQueue = Volley.newRequestQueue(this);
        String url = HostORip + "/user/getAllUserNames";
        JsonArrayRequest jsonreq = new JsonArrayRequest(Request.Method.GET, url, null,
                new Response.Listener<JSONArray>() {
                    @Override
                    public void onResponse(JSONArray response) {
                        try {
                            for (int i = 0; i < response.length(); i++) {
                                JSONObject item = response.getJSONObject(i);

                                String name = item.getString("userName");
                                String email = String.valueOf(item.getString("email"));
                                usernames.add(name);
                                useremails.add(email);
                            }
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


        Bsignedup = findViewById(R.id.Signedup);
        Bsignedup.setOnClickListener(new View.OnClickListener() {
            @Override
            public void onClick(View v) {

                if(usernames.contains(etusername.getText().toString()) || useremails.contains(etmail.getText().toString()))
                {
                    System.out.println(etusername.getText().toString());
                    ErrorDialog error = new ErrorDialog("     Username or Email is taken");
                    error.show(getSupportFragmentManager(), "Error");
                }
                else
                {
                    String data = "{"+
                            "\"userName\":" + "\"" + etusername.getText().toString() + "\", "+
                            "\"password\":" + "\"" + etpassword.getText().toString() + "\", "+
                            "\"email\":" + "\"" + etmail.getText().toString() + "\" "+
                            "}";
                    Submit(data);
                }
            }
        });
    }
    private void Submit(String data)
    {
        final String savedata= data;
        String URL= HostORip + "/user/addNewUser";

        requestQueue = Volley.newRequestQueue(getApplicationContext());
        StringRequest stringRequest = new StringRequest(Request.Method.POST, URL, new Response.Listener<String>() {
            @Override
            public void onResponse(String response) {
                try {
                    JSONObject objres = new JSONObject(response);
                    if (objres.getString("userName").equals(etusername.getText()));
                    {
                        Intent i = new Intent(getApplicationContext(), MainActivity.class);
                        ErrorDialog hello = new ErrorDialog("     Hello " + objres.getString("userName"), i);
                        hello.show(getSupportFragmentManager(), "Hello");
                    }
                } catch (JSONException e) {
                    Toast.makeText(getApplicationContext(),"Server Error",Toast.LENGTH_LONG).show();

                }
                //Log.i("VOLLEY", response);
            }
        }, new Response.ErrorListener() {
            @Override
            public void onErrorResponse(VolleyError error) {

                ErrorDialog error1 = new ErrorDialog("     username or email is already taken ");
                error1.show(getSupportFragmentManager(), "");
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
