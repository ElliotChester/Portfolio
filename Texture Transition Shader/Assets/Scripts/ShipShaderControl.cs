using System.Collections;
using System.Collections.Generic;
using UnityEngine;

public class ShipShaderControl : MonoBehaviour {

    public float TransitionSpeed;
    public float ShipDisolveTarget;
    public float ShipDisolveAmount;

    // Use this for initialization
    void Start () {
		
	}
	
	// Update is called once per frame
	void Update () {
        ShipDisolveAmount = Mathf.MoveTowards(ShipDisolveAmount, ShipDisolveTarget, TransitionSpeed);

        if (Input.GetButtonDown("Fire1"))
        {
            if(ShipDisolveTarget == 1)
            {
                ShipDisolveTarget = 0;
            }else
            if (ShipDisolveTarget == 0)
            {
                ShipDisolveTarget = 1;
            }
        }

        this.GetComponent<Renderer>().material.SetFloat("DissolveAmount", ShipDisolveAmount);

	}
}
